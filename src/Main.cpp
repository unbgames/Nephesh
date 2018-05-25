#include <iostream>
#include <Log.h>
#include <Vec2.h>
#include <Rect.h>
#include <TitleState.h>
#include <chrono>
#include "Game.h"

int main(int argc, char **argv) {
    auto begin = std::chrono::system_clock::now();

    try {
        srand(time(NULL));
        auto &game = Game::GetInstance();
        game.Push(new TitleState());
        game.Run();
    } catch (string ex) {
        Log::LogError(ex);
        return EXIT_FAILURE;
    }

    // https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
    auto executionTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - begin) ;
    cout << "Execution time: "<< (double) executionTime.count()/1000 << " ms" << endl;

    return EXIT_SUCCESS;
}