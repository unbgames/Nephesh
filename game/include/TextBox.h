//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_TEXTBOX_H
#define NEPHESH_TEXTBOX_H


#include "GameObject.h"

#define PADDING_TOP 70
#define PADDING_LEFT 140
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
};


#endif //NEPHESH_TEXTBOX_H
