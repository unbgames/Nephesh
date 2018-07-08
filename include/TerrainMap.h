//
// Created by ftfnunes on 03/07/18.
//

#ifndef NEPHESH_TERRAINMAP_H
#define NEPHESH_TERRAINMAP_H


#include <Component.h>
#include <vector>

#define TERRAIN_MAP_TYPE "TerrainMap"

class TerrainMap : public Component {
public:
    enum TerrainType {
        DIRT,
        STONE,
        GRASS
    };

    TerrainMap(GameObject &associated, string file, int tileWidht, int tileHeight);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Load(string file);

    TerrainType At(int x, int y, int z);

    TerrainType GetCurrentTerrain(Vec2 pos, int layer);

private:
    int tileWidth;
    int tileHeight;

    int mapDepth;
    int mapHeight;
    int mapWidth;
    vector<TerrainType> terrainTilesMatrix;

    TerrainType convertToType(int typeCode);
};


#endif //NEPHESH_TERRAINMAP_H
