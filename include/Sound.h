#ifndef T1_SOUND_H
#define T1_SOUND_H

#define INCLUDE_SDL_MIXER

#include <string>
#include "SDL_include.h"
#include "Component.h"
#include "GameObject.h"

#define SOUND_TYPE "Sound"

class Sound : public Component {
public:
    explicit Sound(GameObject &associated);
    Sound(GameObject &associated, string file);
    ~Sound() override = default;
    void Play(int times = 1);
    void Stop();
    void Open(string file);
    bool IsOpen();

    void Update(float dt) override ;
    void Render() override ;
    bool Is(string type) override ;

private:
    shared_ptr<Mix_Chunk> chunk;
    int channel;
};


#endif //T1_SOUND_H
