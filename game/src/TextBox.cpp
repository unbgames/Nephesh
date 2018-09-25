//
// Created by ftfnunes on 13/06/18.
//

#include <Sprite.h>
#include <Game.h>
#include <Text.h>
#include <CameraFollower.h>
#include <Sound.h>
#include "TextBox.h"
#include "config.h"

TextBox::TextBox(GameObject &associated) : Component(associated), containedText() {
    auto sprite = new Sprite(associated, ASSETS_PATH("/img/textBox.png") );
    associated.box.w = GAME_WIDTH;
    associated.box.h = 300;
    associated.box.y = GAME_HEIGHT - associated.box.h;
    sprite->SetScale(associated.box.w/sprite->GetWidth(), associated.box.h/sprite->GetHeight());
    associated.AddComponent(sprite);
    associated.AddComponent(new CameraFollower(associated, Vec2(0, GAME_HEIGHT - associated.box.h)));
    auto sound = new Sound(associated, ASSETS_PATH("/audio/popup_chat.wav") );
    associated.AddComponent(sound);
    sound->Play();

    associated.box.w = GAME_WIDTH;
}

void TextBox::Update(float dt) {
}

void TextBox::Start() {
}

void TextBox::Render() {

}

bool TextBox::Is(string type) {
    return type == TEXT_BOX_TYPE;
}

void TextBox::SetText(string text) {
    if (containedText.expired()) {
        auto textObj = new GameObject(associated.GetLayer()+1);
        SDL_Color color = {0, 0, 0, 255};
        auto textCpt = new Text(*textObj, ASSETS_PATH("/font/leadcoat.ttf"), 40, Text::TextStyle::SOLID, text, color);
        textObj->AddComponent(textCpt);
        textObj->box.x = PADDING_LEFT;
        textObj->box.y = associated.box.y + PADDING_TOP;
        textObj->AddComponent(new CameraFollower(*textObj, Vec2(textObj->box.x, textObj->box.y)));

        auto &game = Game::GetInstance();
        containedText = game.GetCurrentState().AddObject(textObj);
    } else {
        auto textCpt = (Text *)containedText.lock()->GetComponent(TEXT_TYPE);
        textCpt->SetText(text);
        textCpt->RemakeTexture();
    }
}

void TextBox::DeleteText() {
    containedText.lock()->RequestDelete();
}
