//
// Created by ftfnunes on 07/07/18.
//

#include <Sprite.h>
#include <CameraFollower.h>
#include <Game.h>
#include "Bar.h"

Bar::Bar(GameObject &associated, string spriteFile, int maxValue, int currentValue) : Component(associated),
                                                                                              spriteFile(std::move(spriteFile)),
                                                                                              currentValue(currentValue),
                                                                                              maxValue(maxValue) {

}

void Bar::Update(float dt) {
    auto barSize = (int) associated.box.w * (((float)currentValue)/maxValue);
    auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);

    auto clip = sprite->GetClip();
    sprite->SetClip(clip.x, clip.y, barSize, clip.h);
}

void Bar::Render() {

}

bool Bar::Is(string type) {
    return type == BAR_TYPE;
}

void Bar::Start() {
    associated.AddComponent(new Sprite(associated, spriteFile));
    auto position = Vec2(WIDTH/2 - associated.box.w/2, 50);
    associated.AddComponent(new CameraFollower(associated, position));
}

void Bar::SetValue(int value) {
    currentValue = value;
}
