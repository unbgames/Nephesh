//
// Created by bruno on 08/07/18.
//

#include <cmath>
#include "CameraShaker.h"
#include "Camera.h"

CameraShaker::CameraShaker(GameObject &associated, int shakeSize) :
        Component(associated),
        currentState(IDLE),
        shakerTimer(),
        shakeSize(shakeSize),
        fadingShakeSizePercentage(shakeSize),
        shakeVec(0,0),
        shakeDuration(0),
        fadingSpeed(0) {
}

void CameraShaker::Update(float dt) {
    if(currentState != IDLE && shakerTimer.Get() < shakeDuration){
        shakerTimer.Update(dt);

        auto deltaRotation = CAMERA_SHAKER_ROTATION_SPEED*dt;
        shakeVec.Rotate(deltaRotation);

        if(currentState == FADING){
            fadingShakeSizePercentage -= fadingSpeed*dt;
            Camera::pos += shakeVec*fadingShakeSizePercentage;
        }
        else{
            Camera::pos += shakeVec;
        }

    }
    else if(currentState != IDLE){
        currentState = IDLE;
        shakerTimer.Restart();
    }
}

void CameraShaker::Render() {

}

bool CameraShaker::Is(string type) {
    return type == CAMERA_SHAKER_TYPE;
}

void CameraShaker::SingleShake() {
    GenerateShakeVec();
    Camera::pos += shakeVec;
}

void CameraShaker::KeepShaking(float duration, bool fade) {
    if(fade){
        currentState = FADING;
        fadingShakeSizePercentage = 1;
        fadingSpeed = 1 / shakeDuration;
    }
    else{
        currentState = SHAKING;
    }

    GenerateShakeVec();
}

void CameraShaker::SetShakeSize(int shakeSize) {
    this->shakeSize = shakeSize;
}

int CameraShaker::GetShakeSize() {
    return shakeSize;
}

void CameraShaker::GenerateShakeVec() {
    shakeVec = Vec2(shakeSize, 0).Rotate(M_PI * ((rand() % 1001)/500.0));
}


