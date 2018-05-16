//
// Created by ftfnunes on 17/03/18.
//

#ifndef T1_COMPONENT_H
#define T1_COMPONENT_H

#include <iostream>

using namespace std;

class GameObject;
class Component {
public:
    Component(GameObject& associated);
    virtual ~Component();
    virtual void Update(float dt) = 0;
    virtual void Start();
    virtual void Render() = 0;
    virtual bool Is(string type) = 0;
    virtual void NotifyCollision(GameObject &other);

protected:
    GameObject& associated;
};

#endif //T1_COMPONENT_H
