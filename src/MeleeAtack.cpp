//
// Created by ftfnunes on 13/06/18.
//

#include "MeleeAtack.h"

MeleeAtack::MeleeAtack(GameObject &associated) : Component(associated) {

}

void MeleeAtack::Update(float dt) {

}

void MeleeAtack::Render() {

}

bool MeleeAtack::Is(string type) {
    return false;
}
