#ifndef T1_TILEMAP_H
#define T1_TILEMAP_H

#define TILE_MAP_TYPE "TileMap"

#include <vector>
#include "Component.h"
#include "TileSet.h"

class TileMap : public Component {
public:
    TileMap(GameObject &associated, string file, TileSet *tileSet);

    void Load(string file);
    void SetTileSet(TileSet *tileSet);
    int &At(int x, int y, int z = 0);

    void Render() override;
    void Update(float dt) override;
    bool Is(string type) override;

    void RenderLayer(int layer, int cameraX, int cameraY);
    void RenderLayer(int layer);
    int GetWidth();
    int GetHeight();
    int GetDepth();
private:
    vector<int> tileMatrix;
    TileSet *tileSet;
    int mapWidth;
    int mapHeight;
    int mapDepth;
};


#endif //T1_TILEMAP_H
