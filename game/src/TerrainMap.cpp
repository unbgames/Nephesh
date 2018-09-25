//
// Created by ftfnunes on 03/07/18.
//

#include <fstream>
#include <Game.h>
#include <Player.h>
#include "TerrainMap.h"

TerrainMap::TerrainMap(GameObject &associated, string file, int tileWidth, int tileHeight) : Component(associated),
                                                                                             tileHeight(tileHeight),
                                                                                             tileWidth(tileWidth) {
    Load(file);
}

void TerrainMap::Update(float dt) {

}

void TerrainMap::Render() {

}

bool TerrainMap::Is(string type) {
    return type == TERRAIN_MAP_TYPE;
}

void TerrainMap::Load(string file) {
    ifstream f(ASSETS_PATH + file);
    string line;
    int w, h, d;

    if (f.is_open()) {
        if (!getline(f, line)) {
            throw "Error reading dimensions from " + file;
        }

        if (sscanf(line.c_str(), "%d,%d,%d", &w, &h, &d) != 3) {
            throw "Invalid dimensions on file " + file;
        }

        mapDepth = d;
        mapHeight = h;
        mapWidth = w;

        if (!getline(f, line)) {
            throw "Error while reading from file " + file;
        }

        for (int z = 0; z < d; ++z) {
            for (int i = 0; i < h; ++i) {
                if (!getline(f, line)) {
                    throw "Error while reading from file " + file;
                }
                int j = 0;
                string buff{""};
                for(auto n:line) {
                    if (j == w) break;
                    if(n != ',') buff+=n; else
                    if(!buff.empty()) {
                        j++;
                        terrainTilesMatrix.push_back(convertToType(atoi(buff.c_str())));
                        buff = "";
                    }
                }
                if(!buff.empty()) terrainTilesMatrix.push_back(convertToType(atoi(buff.c_str())));
            }
            if (!getline(f, line) && !line.empty()) {
                throw "Error in file format in " + file;
            }
        }
    } else {
        throw "Erro ao abrir o arquivo " + file;
    }
}

TerrainMap::TerrainType TerrainMap::GetCurrentTerrain(Vec2 pos, int layer) {
    auto x = ((int) (pos.x - associated.box.x))/tileWidth;
    auto y = ((int) (pos.y - associated.box.y))/tileHeight;

    return At(x, y, layer);
}

TerrainMap::TerrainType TerrainMap::At(int x, int y, int z) {
    if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || z < 0 || z >= mapDepth)
        return DIRT;

    int index = x + mapWidth*y + mapWidth*mapHeight*z;
    return terrainTilesMatrix[index];
}

TerrainMap::TerrainType TerrainMap::convertToType(int typeCode) {
    switch (typeCode) {
        case 0:
            return DIRT;
        case 1:
            return GRASS;
        case 2:
            return STONE;
        default:
            return DIRT;
    }
}
