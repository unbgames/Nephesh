//
// Created by bruno on 08/07/18.
//

#ifndef NEPHESH_CAMERASHAKER_H
#define NEPHESH_CAMERASHAKER_H


#include "Component.h"
#include "Vec2.h"
#include "Timer.h"

#define CAMERA_SHAKER_TYPE "CameraShaker"
#define CAMERA_SHAKER_ROTATION_SPEED 1000 //in rad/s

class CameraShaker : public Component {
public:
    enum ShakerState {
        IDLE,
        SHAKING,
        FADING
    };

    explicit CameraShaker(GameObject &associated, int shakeSize = 50);

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void SingleShake();
    void KeepShaking(float duration, bool fade = false);
    void SetShakeSize(int shakeSize);

    int GetShakeSize();

private:
    ShakerState currentState;
    Timer shakerTimer;
    int shakeSize;
    float fadingShakeSizePercentage;
    Vec2 shakeVec;
    float shakeDuration;
    float fadingSpeed;

    void GenerateShakeVec();
};


#endif //NEPHESH_CAMERASHAKER_H
