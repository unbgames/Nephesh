//
// Created by ftfnunes on 07/07/18.
//

#include <Sprite.h>
#include <CameraFollower.h>
#include <Game.h>
#include "Bar.h"

Bar::Bar(GameObject &associated, string spriteFile, int maxValue, int currentValue) : Component(associated),
                                                                                              spriteFile(spriteFile),
                                                                                              currentValue(currentValue),
                                                                                              maxValue(maxValue) {
    associated.AddComponent(new Sprite(associated, spriteFile));
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
}

void Bar::SetValue(int value) {
    currentValue = value;
}
