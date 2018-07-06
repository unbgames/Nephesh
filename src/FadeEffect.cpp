//
// Created by ftfnunes on 04/07/18.
//

#include <Sprite.h>
#include <IntervalTimer.h>
#include <CameraFollower.h>
#include <Player.h>
#include "FadeEffect.h"

FadeEffect::FadeEffect(GameObject &associated, float duration, float offset, function<void()> callback, FadeType type) : Component(associated),
                                                                                                              duration(duration),
                                                                                                              timer(),
                                                                                                              timeElapsed(0),
                                                                                                              offset(offset),
                                                                                                              callback(callback),
                                                                                                              finished(false),
                                                                                                              type(type) {
    auto sprite = new Sprite(associated, "img/black_layer.png");
    associated.AddComponent(sprite);
    associated.AddComponent(new CameraFollower(associated));
    
    if (type == OUT) {
        sprite->SetAlpha(0);
    } else {
        sprite->SetAlpha(255);
    }
}

void FadeEffect::Update(float dt) {
    if (!finished) {
        timer.Update(dt);

        if (timer.Get() > duration) {
            finished = true;
            auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
            sprite->SetAlpha((Uint8) (type == OUT ? 255 : 0));
            callback();
        } else if (timer.Get() > offset) {
            auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
            Uint8 alpha;
            if (type == IN) {
                alpha = (Uint8) (255 * ((duration - timer.Get()) / (duration - offset)));
            } else {
                alpha = (Uint8) (255 * ((timer.Get() - offset) / (duration - offset)));
            }

            sprite->SetAlpha(alpha);
        }
    } else {
        associated.RequestDelete();
    }
}

void FadeEffect::Render() {

}

bool FadeEffect::Is(string type) {
    return type == FADE_IN_EFFECT_TYPE;
}
