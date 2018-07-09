//
// Created by fabio on 08/07/18.
//

#ifndef NEPHESH_DECENTMUSIC_H
#define NEPHESH_DECENTMUSIC_H

#include <iostream>
#include <memory>
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

using namespace std;

class DecentMusic {
public:
    DecentMusic();
    ~DecentMusic();
    DecentMusic(string file);
    void Play(int times = 0, int msToStart = 0);
    void Stop(int msToStop = 1500);
    void Open(string file);
    bool IsOpen();
private:
    shared_ptr<Mix_Chunk> music;
    int channel;
};


#endif //NEPHESH_DECENTMUSIC_H
