#include <iostream>
#include <Resources.h>
#include <InputManager.h>

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "Game.h"

using namespace std;

Game *Game::instance = nullptr;

Game::Game(string title, int width, int height) : dt(0), framestart(0) {
    if (instance == nullptr) {
        instance = this;

        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
            throw "Error initializing SDL";
        }

        // Initialize SDL_IMG
        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
            throw "Error initializing SDL_Image";
        }

        //Initialize SDL_TTF
        if (TTF_Init() != 0) {
            throw "Error initializing SDL_TTF";
        }

        // Initialize SDL_MIXER
        if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
            throw "Error initializing SDL_Mixer";
        }
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
            throw "Error while opening audio";
        }
        Mix_AllocateChannels(32);

        // Create SDL_Window
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, 0);
        if (window == nullptr) {
            throw "Error while creating window";
        }

        // Create SDL_Renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw "Error while creating renderer";
        }

        //Initialize state
        storedState = nullptr;
    } else {
        throw "There are two instances of game created.";
    }
}

Game::~Game() {
    if (storedState != nullptr) {
        delete storedState;
    }

    while (stateStack.empty()) {
        stateStack.pop();
    }

    clearResources();

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Destroy SDL_Mixer
    Mix_CloseAudio();
    Mix_Quit();

    // Destroy SDL_TTF
    TTF_Quit();

    // Destroy SDL_Img
    IMG_Quit();
}

State& Game::GetCurrentState() {
    return *(unique_ptr<State> &)stateStack.top();
}

void Game::Push(State *state) {
    storedState = state;
}

State *Game::push() {
    stateStack.emplace(storedState);
    storedState = nullptr;

    State *state = &GetCurrentState();
    state->Start();
    return state;
}

void Game::Run() {
    if (storedState == nullptr) {
        throw "No initial state";
    }

    push();
    while (!stateStack.empty() && !GetCurrentState().QuitRequested()) {
        State *state = &GetCurrentState();
        if (state->PopRequested()) {
            stateStack.pop();
            clearResources();
            if (!stateStack.empty()) {
                state = &GetCurrentState();
                state->Resume();
            }
        }

        if (storedState != nullptr) {
            if (!stateStack.empty()) {
                state->Pause();
            }
            state = push();
        }

        if (!stateStack.empty()) {
            CalculateDeltaTime();
            InputManager::GetInstance().Update();
            state->Update(dt);
            state->Render();
            SDL_RenderPresent(renderer);
            SDL_Delay(33);
        }
    }
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

Game& Game::GetInstance() {
    if (instance == nullptr) {
        return *new Game(GAME_NAME, WIDTH, HEIGHT);
    }

    return *instance;
}

float Game::GetDeltaTime() {
    return dt;
}

void Game::CalculateDeltaTime() {
    auto ticks = SDL_GetTicks();
    auto deltaTicks = ticks - framestart;
    dt = deltaTicks/1000.0f;
    framestart = ticks;
}

void Game::clearResources() {
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}
