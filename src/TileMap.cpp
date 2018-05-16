#include <fstream>
#include <Game.h>
#include <GameObject.h>
#include <Camera.h>
#include <TileMap.h>

TileMap::TileMap(GameObject &associated, string file, TileSet *tileSet) : Component(associated),
                                                                          tileSet(tileSet) {
    Load(file);
}

void TileMap::SetTileSet(TileSet *tileSet) {
    this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z) {
    int index = x + mapWidth*y + mapWidth*mapHeight*z;
    return tileMatrix[index];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    RenderLayer(layer);
}

void TileMap::RenderLayer(int layer) {
    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; ++j) {
            auto x = i*tileSet->GetTileWidth();
            auto y = j*tileSet->GetTileHeight();
            Rect box = associated.box;
            tileSet->RenderTile(At(i, j, layer), x, y, layer);
        }
    }
}

void TileMap::Render() {
    for (int z = 0; z < mapDepth; ++z) {
       RenderLayer(z, Camera::pos.x, Camera::pos.y);
    }
}

void TileMap::Update(float dt) {}

bool TileMap::Is(string type) {
    return type == TILE_MAP_TYPE;
}

int TileMap::GetHeight() {
    return mapHeight;
}

int TileMap::GetWidth() {
    return mapWidth;
}

int TileMap::GetDepth() {
    return mapDepth;
}

void TileMap::Load(string file) {
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
                        int c = atoi(buff.c_str())-1;
                        j++;
                        tileMatrix.push_back(c);
                        buff = "";
                    }
                }
                if(!buff.empty()) tileMatrix.push_back(atoi(buff.c_str())-1);
            }
            if (!getline(f, line) && !line.empty()) {
                throw "Error in file format in " + file;
            }
        }
    } else {
        throw "Erro ao abrir o arquivo " + file;
    }
}
