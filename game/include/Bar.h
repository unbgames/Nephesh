//
// Created by ftfnunes on 07/07/18.
//

#ifndef NEPHESH_BAR_H
#define NEPHESH_BAR_H

#define BAR_TYPE "Bar"

#include <GameObject.h>

class Bar : public Component {
public:
    Bar(GameObject &associated, string spriteFile, int maxValue, int currentValue);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Start() override;

    void SetValue(int value);
private:
    int currentValue;
    int maxValue;

    string spriteFile;

};


#endif //NEPHESH_BAR_H
