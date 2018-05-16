#include <iostream>
#include <memory>
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"

using namespace std;

#ifndef SPRITE_CLASS
#define SPRITE_CLASS

#define SPRITE_TYPE "Sprite"

class Sprite : public Component {
public:
    Sprite(GameObject &associated);
    explicit Sprite(GameObject &associated, string file, int frameCount = 1, float frameTime = 1, float secondsToSelfDestruct = 0);
    ~Sprite() override;

    void Update(float dt);
    bool Is(string type);

    void Open(string file);
    void SetClip(int x, int y, int w, int h);
    void SetClip(int x, int y);
    void Render() override;
    void Render(float x, float y);
    void Render(float x, float y, int layer);
    int GetWidth();
    int GetHeight();
    void SetScaleX(float scaleX, float scaleY);
    Vec2 GetScale();
    bool IsOpen();
    void SetFrame(int frame);
    void SetFrameCount(int frameCount);
    void SetFrameTime(int frameTime);
private:
    shared_ptr<SDL_Texture> texture;
    SDL_Rect clipRect;
    int width;
    int height;
    int frameCount;
    int currentFrame;
    Vec2 scale;
    float timeElapsed;
    float frameTime;
    Timer selfDestructCount;
    float secondsToSelfDestruct;

    int getFrameWidth();
};

#endif