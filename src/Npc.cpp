//
// Created by ftfnunes on 13/06/18.
//

#include <Collidable.h>
#include <Player.h>
#include <TextBox.h>
#include <Game.h>
#include <InputManager.h>
#include <fstream>
#include <Sound.h>
#include "Npc.h"

Npc::Npc(GameObject &associated, string file, bool debug) : Component(associated), isTalking(false), debug(debug) {
    associated.AddComponent(new Collidable(associated, Vec2(1, 1)*2));

    ReadSpeeches(file);
}

void Npc::Update(float dt) {
    if (debug) {
        auto &inputManager = InputManager::GetInstance();

        if (inputManager.IsKeyDown(UP_ARROW_KEY)) {
            associated.box.y -= 1;
        }
        if (inputManager.IsKeyDown(DOWN_ARROW_KEY)) {
            associated.box.y += 1;
        }
        if (inputManager.IsKeyDown(LEFT_ARROW_KEY)) {
            associated.box.x -= 1;
        }
        if (inputManager.IsKeyDown(RIGHT_ARROW_KEY)) {
            associated.box.x += 1;
        }

        if (inputManager.KeyPress(SDLK_p)) {
            cout << associated.box.x << ", " << associated.box.y << endl;
        }
    }

    if (isTalking && InputManager::GetInstance().KeyPress(SPACE_KEY)) {
        auto boxCpt = (TextBox *) textBox.lock()->GetComponent(TEXT_BOX_TYPE);
        if (speechQueue.empty()) {
            Player::player->StopTalking();
            boxCpt->DeleteText();
            textBox.lock()->RequestDelete();
            isTalking = false;
        } else {
            auto sound = (Sound *)associated.GetComponent(SOUND_TYPE);
            if (sound != nullptr) {
                sound->Play();
            }

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
    
    if (!getline(stream, line)) {
        throw "Invalid Format of file " + file;
    }
    int x, y;
    if (sscanf(line.c_str(), "%d,%d", &x, &y) != 2) {
        throw "Invalid format of file " + file;
    }

    associated.box.x = x;
    associated.box.y = y;

    while (getline(stream, line)) {
        speechLines.push_back(line);
    }

    stream.close();
}

void Npc::Talk() {
    isTalking = true;

    RestartLines();

    auto textBoxObj = new GameObject(1);
    auto box = new TextBox(*textBoxObj);
    textBoxObj->AddComponent(box);
    textBox = Game::GetInstance().GetCurrentState().AddObject(textBoxObj);

    box->SetText(speechLines.front());
    speechQueue.pop();

    Player::player->closestNpc = weak_ptr<GameObject>();
}
