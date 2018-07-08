#ifndef T1_TILESET_H
#define T1_TILESET_H

#include <iostream>
#include "Sprite.h"
#include "GameObject.h"

using namespace std;

#define TILE_DIMENSIONS 64

class TileSet {
public:
    TileSet(int tileWidth, int tileHeight, string file);

    void RenderTile(unsigned int index, float x, float y);
    void RenderTile(unsigned int index, float x, float y, int layer);
    int GetTileWidth();
    int GetTileHeight();

private:
    Sprite tileSet;
    int rows;
    int columns;
    int tileWidth;
    int tileHeight;
};


#endif //T1_TILESET_H
