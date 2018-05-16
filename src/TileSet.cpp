#include "TileSet.h"
#include "GameObject.h"

//TODO: how to pass gameobject to sprite
TileSet::TileSet(int tileWidth, int tileHeight, string file) : tileWidth(tileWidth),
                                                               tileHeight(tileHeight),
                                                               tileSet(*new GameObject(), file) {
    columns = tileSet.GetWidth() / tileWidth;
    rows = tileSet.GetHeight() / tileHeight;
}

void TileSet::RenderTile(unsigned int index, float x, float y) {
    RenderTile(index, x, y, 0);
}


void TileSet::RenderTile(unsigned int index, float x, float y, int layer) {
    int maxRange = columns*rows;
    if (index < maxRange-1) {
        int column = index % columns;
        int row = index / columns;
        tileSet.SetClip(column*tileWidth, row*tileHeight, tileWidth, tileHeight);
        tileSet.Render(x, y, layer);
    }
}

int TileSet::GetTileHeight() {
    return tileHeight;
}

int TileSet::GetTileWidth() {
    return tileWidth;
}

