//
// Created by ftfnunes on 13/06/18.
//

#include <Sprite.h>
#include <Game.h>
#include <Text.h>
#include <CameraFollower.h>
#include "TextBox.h"

TextBox::TextBox(GameObject &associated) : Component(associated), containedText() {
    auto sprite = new Sprite(associated, "img/textBox.png");
    associated.box.w = WIDTH;
    associated.box.h = 200;
    associated.box.y = HEIGHT - associated.box.h;
    sprite->SetScale(associated.box.w/sprite->GetWidth(), associated.box.h/sprite->GetHeight());
    associated.AddComponent(sprite);
    associated.AddComponent(new CameraFollower(associated, Vec2(0, HEIGHT - associated.box.h)));

    associated.box.w = WIDTH;
}

void TextBox::Update(float dt) {

}

void TextBox::Start() {
    auto s = "ahsjdhakjsdhkjashdjkashdjkhasjkdhaskdhasjkhdhjaskjhdhjkaaaaaaaaaaaaaa";
    SetText(s);
}

void TextBox::Render() {

}

bool TextBox::Is(string type) {
    return false;
}

void TextBox::SetText(string text) {
    if (containedText.expired()) {
        auto textObj = new GameObject(associated.GetLayer()+1);
        SDL_Color color = {0, 0, 0, 255};
        auto textCpt = new Text(*textObj, "font/leadcoat.ttf", 30, Text::TextStyle::SOLID, text, color);
        textObj->AddComponent(textCpt);
        textObj->box.x = PADDING_LEFT;
        textObj->box.y = associated.box.y + PADDING_TOP;

        auto &game = Game::GetInstance();
        containedText = game.GetCurrentState().AddObject(textObj);
    } else {
        auto textCpt = (Text *)containedText.lock()->GetComponent(TEXT_TYPE);
        textCpt->SetText(text);
    }
}
