//
// Created by ftfnunes on 03/05/18.
//

#include "State.h"

State::State() : popRequested(false), quitRequested(false), started(false) {}

State::~State() {
    objectLayers.clear();
}

weak_ptr<GameObject> State::AddObject(GameObject *obj) {
    auto ptr = shared_ptr<GameObject>(obj);

    return AddObject(ptr);
}

weak_ptr<GameObject> State::AddObject(shared_ptr<GameObject> ptr) {

    objectLayers[(*ptr).GetLayer()].push_back(ptr);

    if (started && !(*ptr).HasStarted()) {
        (*ptr).Start();
    }

    addedObjects.emplace_back(ptr);
    return weak_ptr<GameObject>(ptr);
}

weak_ptr<GameObject> State::GetObjectPtr(GameObject *obj) {

    auto &objects = objectLayers[obj->GetLayer()];
    for (auto &object : objects) {
        if (object.get() == obj) {
            return weak_ptr<GameObject>(object);
        }
    }

    return weak_ptr<GameObject>();
}

shared_ptr<GameObject> State::PopObjectPtr(GameObject *obj) {
    auto &layer = objectLayers[obj->GetLayer()];

    for (int i = 0; i < layer.size(); ++i) {
        auto object = layer[i];
        if (object.get() == obj) {
            layer.erase(layer.begin() + i);
            return object;
        }
    }

    return shared_ptr<GameObject>();
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
}

void State::StartArray() {
    this->LoadAssets();
    for (auto &objLayer : objectLayers) {
        auto &objects = objLayer.second;
        for (int i = 0; i < objects.size(); i++) {
            (*objects[i]).Start();
        }
    }

    started = true;
}

void State::UpdateArray(float dt) {
    for (auto &it : objectLayers) {
        auto &objects = it.second;

        for (int i = 0; i < objects.size(); ++i) {
            auto obj = objects[i];
            if (!obj->IsUpdated()) {
                obj->Update(dt);
            }
        }
    }

    for (int i = 0; i < addedObjects.size(); ++i) {
        auto objPtr = addedObjects[i].lock();
        if (!objPtr->IsUpdated()) {
            objPtr->Update(dt);
        }
    }
    addedObjects.clear();
}

void State::RenderArray() {
    for (int i = 0; i < objectLayers.size(); i++) {
        auto it = objectLayers.find(i);

        if (it != objectLayers.end()) {
            auto &objects = (*it).second;

            for (auto &object : objects) {
                object->Render();
            }
        }
    }
}
