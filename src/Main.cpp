#include <iostream>
#include <Log.h>
#include <Vec2.h>
#include <Rect.h>
#include <TitleState.h>
#include "Game.h"

int main(int argc, char **argv) {
    try {
        srand(time(NULL));
        auto &game = Game::GetInstance();
        game.Push(new TitleState());
        game.Run();
    } catch (string ex) {
        Log::LogError(ex);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}