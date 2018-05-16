#include <algorithm>
#include <Game.h>
#include "GameObject.h"

GameObject::GameObject() : GameObject(0) {}


GameObject::GameObject(int layer) : isDead(false),
                                    started(false),
                                    updated(false),
                                    angleDeg(0),
                                    layer(layer) {}

GameObject::~GameObject() {
    for(auto it = components.rbegin(); it != components.rend(); ++it) {
        (*it).reset();
    }

    components.clear();
}

void GameObject::Update(float dt) {
    for(int i = 0; i < components.size(); ++i) {
        components[i]->Update(dt);
    }

    updated = true;
}

void GameObject::Render() {
    updated = false;
    for(auto it = components.begin(); it != components.end(); ++it) {
        (*it)->Render();
    }
}

bool GameObject::IsDead() {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component *cpt) {
    if (started) {
        cpt->Start();
    }
    components.emplace_back(cpt);
}

void GameObject::RemoveComponent(Component *cpt) {
    unique_ptr<Component>& p = *new unique_ptr<Component>(cpt);
    components.erase(std::remove(components.begin(), components.end(), p), components.end());
}

Component *GameObject::GetComponent(string type) {
    for(auto &it : components) {
        if (it->Is(type)) {
            return it.get();
        }
    }
    return nullptr;
}

void GameObject::Start() {
    for(auto &it : components) {
        (*it).Start();
    }
    started = true;
}

int GameObject::GetLayer() {
    return layer;
}

void GameObject::SetLayer(int layer) {
    auto &state = Game::GetInstance().GetCurrentState();
    auto obj = state.PopObjectPtr(this);
    this->layer = layer;
    if (obj) {
        state.AddObject(obj);
    }
}

bool GameObject::HasStarted() {
    return started;
}

bool GameObject::IsUpdated() {
    return updated;
}

void GameObject::NotifyCollision(GameObject &other) {
    for (auto &it : components) {
        (*it).NotifyCollision(other);
    }
}

bool GameObject::HasComponent(string type) {
    for(auto &it : components) {
        if (it->Is(type)) {
            return true;
        }
    }
    return false;
}

void GameObject::SetCenter(Vec2 centerPos) {
    box.x = centerPos.x - box.w/2;
    box.y = centerPos.y - box.h/2;
}
