//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_TEXTBOX_H
#define NEPHESH_TEXTBOX_H


#include "GameObject.h"

#define PADDING_TOP 40
#define PADDING_LEFT 70
#define MAX_CHAR_LINE 70

class TextBox : public Component {
public:
    TextBox(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void SetText(string text);

    void Start() override;

private:
    weak_ptr<GameObject> containedText;
};


#endif //NEPHESH_TEXTBOX_H
