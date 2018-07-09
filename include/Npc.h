//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_NPC_H
#define NEPHESH_NPC_H

#include <queue>
#include <memory>
#include "Component.h"

#define NPC_TYPE "Npc"

#define MALE_BOX "img/text_box_religioso.png"
#define FEMALE_BOX "img/text_box_religiosa.png"

class Npc : public Component {
public:
    Npc(GameObject &associated, string file, vector<string> sounds, bool debug = false, bool isReligious = false);

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
    bool isReligious;
    bool isHim;

    string openedFile;

    void RestartLines();

    void ReadSpeeches(string file);
};

#endif //NEPHESH_NPC_H
