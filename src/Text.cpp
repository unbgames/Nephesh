//
// Created by ftfnunes on 08/05/18.
//

#include <Game.h>
#include <Camera.h>
#include <Resources.h>
#include <codecvt>
#include "Text.h"

Text::Text(GameObject &associated,
           string fontFile,
           int fontSize,
           Text::TextStyle style,
           string text,
           SDL_Color color): Component(associated),
                             fontFile(fontFile),
                             fontSize(fontSize),
                             style(style),
                             text(text),
                             color(color),
                             textures() {
    RemakeTexture();
}


Text::~Text() {
    for (auto &texture : textures) {
        SDL_DestroyTexture(texture);
    }

    textures.clear();
}

void Text::Update(float dt) { }

void Text::Render() {
    Game &game = Game::GetInstance();
    auto box = associated.box;
    auto renderPos = Camera::GetRenderPosition(0, Vec2(box.x, box.y));
    SDL_Point center = { (int) associated.rotationCenter.x, (int) associated.rotationCenter.y };
    int i = 0;
    for (auto &texture : textures) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect clipRect = { 0, 0, w, h };
        SDL_Rect dstRect = { (int)renderPos.x, (int)renderPos.y + i*(clipRect.h + 20), clipRect.w, clipRect.h };
        SDL_RenderCopyEx(game.GetRenderer(),
                         texture,
                         &clipRect,
                         &dstRect,
                         associated.angleDeg,
                         &center,
                         SDL_FLIP_NONE);
        i++;
    }
}

bool Text::Is(string type) {
    return type == TEXT_TYPE;
}

void Text::SetText(string text) {
    this->text = text;
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
}

void Text::SetStyle(Text::TextStyle style) {
    this->style = style;
}

void Text::SetFontFile(string fontFile) {
    this->fontFile = fontFile;
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
}

void Text::RemakeTexture() {
    if (textures.size() != 0) {
        for (auto &texture : textures) {
            SDL_DestroyTexture(texture);
        }
    }
    textures.clear();

    font = Resources::GetFont(fontFile, fontSize);

    auto lines = GetLines();

    for (auto &line: lines) {
        SDL_Surface *surface = nullptr;
        switch (style) {
            case TextStyle::SOLID:
                surface = TTF_RenderUTF8_Solid(font.get(), line.c_str(), color);
                break;
            case TextStyle::BLENDED:
                surface = TTF_RenderUTF8_Blended(font.get(), line.c_str(), color);
                break;
            case TextStyle::SHADED:
                SDL_Color black = { 0, 0, 0, 255 };
                surface = TTF_RenderText_Shaded(font.get(), line.c_str(), color, black);
                break;
        }

        auto texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
        SDL_SetTextureAlphaMod(texture, color.a);

        textures.push_back(texture);
        SDL_FreeSurface(surface);
    }
}

SDL_Color Text::GetColor() {
    return color;
}

vector<string> Text::GetLines() {
    int NumSubstrings = text.length() / MAX_CHAR_PER_LINE;
    vector<string> ret;


    for (auto i = 0; i < NumSubstrings; i++)
    {
        ret.push_back(text.substr(i * MAX_CHAR_PER_LINE, MAX_CHAR_PER_LINE));
    }

    // If there are leftover characters, create a shorter item at the end.
    if (text.length() % MAX_CHAR_PER_LINE != 0)
    {
        ret.push_back(text.substr(MAX_CHAR_PER_LINE * NumSubstrings));
    }

    return ret;
}

