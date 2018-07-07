//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_NPC_H
#define NEPHESH_NPC_H

#include <queue>
#include <memory>
#include "Component.h"

#define NPC_TYPE "Npc"

class Npc : public Component {
public:
    Npc(GameObject &associated, string file, vector<string> sounds, bool debug = false);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Talk();
private:
    bool isTalking;
    bool debug;
    vector<string> speechLines;
    queue<string> speechQueue;
    weak_ptr<GameObject> textBox;
    vector<string> sounds;
    int currentSound;

    string openedFile;

    void RestartLines();

    void ReadSpeeches(string file);
};

#endif //NEPHESH_NPC_H
