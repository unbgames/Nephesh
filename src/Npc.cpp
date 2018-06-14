//
// Created by ftfnunes on 13/06/18.
//

#include <Collidable.h>
#include <Player.h>
#include <TextBox.h>
#include <Game.h>
#include <InputManager.h>
#include <fstream>
#include "Npc.h"

Npc::Npc(GameObject &associated, string file) : Component(associated), isTalking(false) {
    associated.AddComponent(new Collidable(associated, Vec2(1, 1)*2));

    ReadSpeeches(file);
}

void Npc::Update(float dt) {
    if (isTalking && InputManager::GetInstance().KeyPress(SPACE_KEY)) {
        auto boxCpt = (TextBox *) textBox.lock()->GetComponent(TEXT_BOX_TYPE);
        if (speechQueue.empty()) {
            Player::player->StopTalking();
            boxCpt->DeleteText();
            textBox.lock()->RequestDelete();
            isTalking = false;
        } else {
            boxCpt->SetText(speechQueue.front());
            speechQueue.pop();
        }
    }
}

void Npc::Render() {

}

bool Npc::Is(string type) {
    return type == NPC_TYPE;
}

void Npc::RestartLines() {
    queue<string>().swap(speechQueue);

    for (auto speech : speechLines) {
        speechQueue.push(speech);
    }
}

void Npc::ReadSpeeches(string file) {
    ifstream stream(ASSETS_PATH + file);
    openedFile = file;
    if (!stream.is_open()) {
        throw "Impossible to open file: " + file;
    }

    string line;

    while (getline(stream, line)) {
        speechLines.push_back(line);
    }

    stream.close();
}

void Npc::Talk() {
    isTalking = true;
    cout << openedFile << endl;

    RestartLines();

    auto textBoxObj = new GameObject(1);
    auto box = new TextBox(*textBoxObj);
    textBoxObj->AddComponent(box);
    textBox = Game::GetInstance().GetCurrentState().AddObject(textBoxObj);

    box->SetText(speechLines.front());
    speechQueue.pop();

    Player::player->closestNpc = weak_ptr<GameObject>();
}
