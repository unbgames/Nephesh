//
// Created by ftfnunes on 13/06/18.
//

#include <Collidable.h>
#include <Player.h>
#include "Npc.h"

Npc::Npc(GameObject &associated) : Component(associated), isTalking(false) {
    associated.AddComponent(new Collidable(associated, Vec2(1, 1)*3));
}

void Npc::Update(float dt) {
    if (Player::player->IsTalking()) {
        if (!isTalking) {
            isTalking = true;
        } else {

        }
    }
}

void Npc::Render() {

}

bool Npc::Is(string type) {
    return type == NPC_TYPE;
}
