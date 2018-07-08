//
// Created by ftfnunes on 08/07/18.
//

#ifndef NEPHESH_DEBUG_H
#define NEPHESH_DEBUG_H


#include <GameObject.h>

class Debug : public Component {
    Vec2 offset;
public:
    explicit Debug(GameObject &associated, Vec2 offset = {0, 0});

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;
};


#endif //NEPHESH_DEBUG_H
