#define INCLUDE_SDL_IMAGE

#include <Game.h>
#include <GameObject.h>
#include <Resources.h>
#include <Camera.h>
#include <Sprite.h>

Sprite::Sprite(GameObject &associated) : Component(associated), scale(Vec2(1, 1)), frameCount(0), frameTime(0), timeElapsed(0), currentFrame(0) {
    texture = nullptr;
}

Sprite::Sprite(GameObject &associated, string file, int frameCount, float frameTime, float secondsToSelfDestruct) : Sprite(associated) {
    this->frameCount = frameCount;
    this->frameTime = frameTime;
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    Open(file);
}

Sprite::~Sprite() = default;

void Sprite::Open(string file) {
    texture = Resources::GetImage(file);

    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);

    associated.box.h = GetHeight();
    associated.box.w = GetWidth();

    SetClip(0, 0, getFrameWidth(), height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}


void Sprite::SetClip(int x, int y) {
    SetClip(x, y, clipRect.w, clipRect.h);
}

void Sprite::Render(float x, float y, int layer) {
    Game &game = Game::GetInstance();
    auto layerScale = Camera::GetLayerScale(layer);
    auto renderPos = Camera::GetRenderPosition(Vec2(x, y), layerScale);
    SDL_Rect dstRect = { renderPos.x, renderPos.y, (int)(clipRect.w*scale.x*layerScale)+1, (int)(clipRect.h*scale.y*layerScale)+1 };
    SDL_RenderCopyEx(game.GetRenderer(),
                     texture.get(),
                     &clipRect,
                     &dstRect,
                     associated.angleDeg,
                     nullptr,
                     SDL_FLIP_NONE);
}

void Sprite::Render(float x, float y) {
    Render(x, y, associated.GetLayer());
}

void Sprite::Render() {
    auto box = associated.box;

    Render(box.x, box.y);
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}

int Sprite::GetHeight() {
    return height*scale.y;
}

bool Sprite::Is(string type) {
    return type == SPRITE_TYPE;
}

void Sprite::Update(float dt) {
    timeElapsed += dt;

    if (secondsToSelfDestruct > 0) {
        selfDestructCount.Update(dt);
        if (selfDestructCount.Get() > secondsToSelfDestruct) {
            associated.RequestDelete();
        }
    }

    if (timeElapsed >= frameTime) {
        auto nextFrame = currentFrame+1;
        if (nextFrame == frameCount) {
            nextFrame = 0;
        }
        SetFrame(nextFrame);
        timeElapsed = 0;
    }
}

int Sprite::GetWidth() {
    return getFrameWidth()*scale.x;
}

void Sprite::SetScaleX(float scaleX, float scaleY) {
    scale.x = scaleX == 0 ? scale.x : scaleX;
    scale.y = scaleY == 0 ? scale.y : scaleY;
    auto &box = associated.box;
    auto center = box.Center();
    box.w = GetWidth();
    box.h = GetHeight();
    box.x = center.x - box.w/2;
    box.y = center.y - box.h/2;
}

Vec2 Sprite::GetScale() {
    return scale;
}

void Sprite::SetFrame(int frame) {
    currentFrame = frame;

    SetClip(getFrameWidth()*frame, 0);
}

void Sprite::SetFrameCount(int frameCount) {
    this->frameCount = frameCount;

    associated.box.w = GetWidth();
    SetClip(0, clipRect.y, getFrameWidth(), clipRect.h);
}

void Sprite::SetFrameTime(int frameTime) {
    this->frameTime = frameTime;
}

int Sprite::getFrameWidth() {
    return width/frameCount;
}


