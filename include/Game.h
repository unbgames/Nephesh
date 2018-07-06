#define INCLUDE_SDL

#include <stack>
#include <memory>
#include "SDL_include.h"
#include "State.h"

#define GAME_NAME "Nephesh"
#define WIDTH 1366
#define HEIGHT 768

#define ASSETS_PATH "assets/"

#ifndef GAME_CLASS
#define GAME_CLASS

class Game {
  public:
    ~Game();
    static Game& GetInstance();
    void Run();
    SDL_Renderer *GetRenderer();

    State &GetCurrentState();

    void Push(State *state);

    float GetDeltaTime();
  private:
    static Game *instance;

    Game(string title, int width, int height);
    void CalculateDeltaTime();
    State *push();
    void clearResources();

    State *storedState;
    stack<unique_ptr<State>> stateStack;

    SDL_Window *window;
    SDL_Renderer *renderer;
    int framestart;
    float dt;
};

#endif