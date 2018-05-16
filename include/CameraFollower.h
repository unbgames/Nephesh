//
// Created by ftfnunes on 30/03/18.
//

#ifndef T1_CAMERAFOLLOWER_H
#define T1_CAMERAFOLLOWER_H


#include "Component.h"

#define CAMERA_FOLLOWER_TYPE "CameraFollower"

class CameraFollower : public Component {
public:
    explicit CameraFollower(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

};


#endif //T1_CAMERAFOLLOWER_H
