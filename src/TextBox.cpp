//
// Created by ftfnunes on 13/06/18.
//

#include <Sprite.h>
#include <Game.h>
#include <Text.h>
#include <CameraFollower.h>
#include <Sound.h>
#include "TextBox.h"

TextBox::TextBox(GameObject &associated) : Component(associated), containedText() {
    auto sprite = new Sprite(associated, "img/text_box.png");
    associated.box.y = GAME_HEIGHT - associated.box.h;
    associated.AddComponent(sprite);
    offset = Vec2(GAME_WIDTH/2 - associated.box.w/2, GAME_HEIGHT - associated.box.h - 10);
    associated.AddComponent(new CameraFollower(associated, offset));
    auto sound = new Sound(associated, "audio/popup_chat.wav");
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
        auto textCpt = new Text(*textObj, DEFAULT_FONT, 20, Text::TextStyle::SOLID, text, color);
        textObj->AddComponent(textCpt);
        textObj->box.x = offset.x + PADDING_LEFT;
        textObj->box.y = offset.y + PADDING_TOP;
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
