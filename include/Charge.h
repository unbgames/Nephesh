//
// Created by ftfnunes on 06/07/18.
//

#ifndef NEPHESH_CHARGE_H
#define NEPHESH_CHARGE_H


#include <GameObject.h>
#include <Timer.h>

#define CHARGE_TYPE "Charge"

class Charge : public Component {
public:
    explicit Charge(GameObject &associated, GameObject *next, float duration);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    Timer chargeTimer;
    GameObject *next;
    float duration;
};


#endif //NEPHESH_CHARGE_H
