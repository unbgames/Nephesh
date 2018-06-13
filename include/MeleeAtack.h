//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_MELEEATACK_H
#define NEPHESH_MELEEATACK_H


#include "Component.h"

class MeleeAtack : public Component {
public:
    MeleeAtack(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

};


#endif //NEPHESH_MELEEATACK_H
