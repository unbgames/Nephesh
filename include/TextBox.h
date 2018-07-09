//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_TEXTBOX_H
#define NEPHESH_TEXTBOX_H


#include "GameObject.h"

#define PADDING_TOP 110
#define PADDING_LEFT 65
#define DEFAULT_FONT "font/PressStart2P-Regular.ttf"
#define TEXT_BOX_TYPE "TextBox"

class TextBox : public Component {
public:
    TextBox(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void SetText(string text);

    void Start() override;

    void DeleteText();
private:
    weak_ptr<GameObject> containedText;
    Vec2 offset;
};


#endif //NEPHESH_TEXTBOX_H
