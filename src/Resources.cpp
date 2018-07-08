#include <Game.h>
#include <memory>
#include "Resources.h"

unordered_map<string, shared_ptr<SDL_Texture>> Resources::imageTable;
unordered_map<string, shared_ptr<Mix_Music>> Resources::musicTable;
unordered_map<string, shared_ptr<Mix_Chunk>> Resources::soundTable;
unordered_map<string, shared_ptr<TTF_Font>> Resources::fontTable;

shared_ptr<SDL_Texture> Resources::GetImage(string file) {
    auto it = imageTable.find(file);
    if (it != imageTable.end()) {
        return (*it).second;
    } else {
        auto texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), (ASSETS_PATH + file).c_str());
        if (texture == nullptr) {
            throw "Error loading texture from image: " + file + " reason: " + SDL_GetError();
        }
        auto texturePtr = shared_ptr<SDL_Texture>(texture, [] (SDL_Texture *texture) -> void {
            SDL_DestroyTexture(texture);
        });
        imageTable.insert(make_pair(file, texturePtr));
        return texturePtr;
    }
}

void Resources::ClearImages() {
    for (auto it = imageTable.begin(); it != imageTable.end();) {
        if (((*it).second).unique()) {
            it = imageTable.erase(it);
        } else {
            ++it;
        }
    }
}

shared_ptr<Mix_Music> Resources::GetMusic(string file) {
    auto it = musicTable.find(file);
    if (it != musicTable.end()) {
        return (*it).second;
    } else {
        auto f = (ASSETS_PATH + file);
        auto music = Mix_LoadMUS(f.c_str());
        if (music == nullptr) {
            throw "Error loading backgroundMusic: " + file + ". Reason: " + string(SDL_GetError());
        }

        auto musicPtr = shared_ptr<Mix_Music>(music, [] (Mix_Music *music) -> void {
            Mix_FreeMusic(music);
        });

        musicTable.insert(make_pair(file, musicPtr));
        return musicPtr;
    }
}

void Resources::ClearMusics() {
    for (auto it = musicTable.begin(); it != musicTable.end();) {
        if (((*it).second).unique()) {
            it = musicTable.erase(it);
        } else {
            ++it;
        }
    }
}

shared_ptr<Mix_Chunk> Resources::GetSound(string file) {
    auto it = soundTable.find(file);
    if (it != soundTable.end()) {
        return (*it).second;
    } else {
        auto chunk = Mix_LoadWAV((ASSETS_PATH + file).c_str());
        if (chunk == nullptr) {
            throw "Error loading file: " + file + ". Reason: " + string(SDL_GetError());
        }

        auto chunkPtr = shared_ptr<Mix_Chunk>(chunk, [] (Mix_Chunk *sound) -> void {
            Mix_FreeChunk(sound);
        });

        soundTable.insert(make_pair(file, chunkPtr));
        return chunkPtr;
    }
}

void Resources::ClearSounds() {
    for (auto it = soundTable.begin(); it != soundTable.end();) {
        if (((*it).second).unique()) {
            it = soundTable.erase(it);
        } else {
            ++it;
        }
    }
}

shared_ptr<TTF_Font> Resources::GetFont(string file, int size) {
    auto key = file + to_string(size);
    auto it = fontTable.find(key);
    if (it != fontTable.end()) {
        return (*it).second;
    } else {
        auto font = TTF_OpenFont((ASSETS_PATH + file).c_str(), size);
        if (font == nullptr) {
            throw "Error loading file: " + file + ". Reason: " + string(SDL_GetError());
        }

        auto fontPtr = shared_ptr<TTF_Font>(font, [] (TTF_Font *font) -> void {
            TTF_CloseFont(font);
        });

        fontTable.insert(make_pair(key, fontPtr));
        return fontPtr;
    }
}

void Resources::ClearFonts() {
    for (auto it = fontTable.begin(); it != fontTable.end();) {
        if (((*it).second).unique()) {
            it = fontTable.erase(it);
        } else {
            ++it;
        }
    }
}
