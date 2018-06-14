//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_NPC_H
#define NEPHESH_NPC_H

#include "Component.h"

#define NPC_TYPE "Npc"

class Npc : public Component {
public:
    explicit Npc(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    bool isTalking;
};

#endif //NEPHESH_NPC_H
