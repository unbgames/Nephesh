//
// Created by ftfnunes on 08/05/18.
//

#include <Game.h>
#include <Camera.h>
#include <Resources.h>
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
                             texture(nullptr) {
    RemakeTexture();
}


Text::~Text() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt) { }

void Text::Render() {
    Game &game = Game::GetInstance();
    auto box = associated.box;
    auto renderPos = Camera::GetRenderPosition(0, Vec2(box.x, box.y));
    SDL_Rect clipRect = { 0, 0, (int) box.w, (int) box.h };
    SDL_Rect dstRect = { renderPos.x, renderPos.y, clipRect.w, clipRect.h };
    SDL_RenderCopyEx(game.GetRenderer(),
                     texture,
                     &clipRect,
                     &dstRect,
                     associated.angleDeg,
                     nullptr,
                     SDL_FLIP_NONE);
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
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }

    font = Resources::GetFont(fontFile, fontSize);

    SDL_Surface *surface = nullptr;
    switch (style) {
        case TextStyle::SOLID:
            surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
            break;
        case TextStyle::BLENDED:
            surface = TTF_RenderText_Blended(font.get(), text.c_str(), color);
            break;
        case TextStyle::SHADED:
            SDL_Color black = { 0, 0, 0, 255 };
            surface = TTF_RenderText_Shaded(font.get(), text.c_str(), color, black);
            break;
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_SetTextureAlphaMod(texture, color.a);
    associated.box.w = surface->w;
    associated.box.h = surface->h;
    SDL_FreeSurface(surface);
}

SDL_Color Text::GetColor() {
    return color;
}

