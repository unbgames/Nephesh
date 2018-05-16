#ifndef T1_TEXT_H
#define T1_TEXT_H



#define INCLUDE_SDL
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include <Component.h>
#include <bits/shared_ptr.h>

#define TEXT_TYPE "Text"

class Text : public Component {
public:
    enum TextStyle {SOLID, SHADED, BLENDED};

    Text(GameObject &associated, string fontFile, int fontSize, TextStyle style, string text, SDL_Color color);
    ~Text() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void SetText(string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(string fontFile);
    void SetFontSize(int fontSize);

    SDL_Color GetColor();

    void RemakeTexture();
private:
    shared_ptr<TTF_Font> font;
    SDL_Texture *texture;

    string text;
    TextStyle style;
    string fontFile;
    int fontSize;
    SDL_Color color;

};


#endif //T1_TEXT_H
