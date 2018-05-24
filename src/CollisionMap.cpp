//
// Created by bruno on 22/05/18.
//

#include <Game.h>
#include <fstream>
#include "CollisionMap.h"

CollisionMap::CollisionMap(GameObject &associated, string file, int tileWidth, int tileHeight) :
        Collidable(associated),
        tileWidth(tileWidth),
        tileHeight(tileHeight) {
    Load(file);
}

void CollisionMap::Update(float dt) {

}

void CollisionMap::Render() {

}

bool CollisionMap::Is(string type) {
    return type == COLLIDABLE_TYPE || type == COLLISION_MAP_TYPE;
}

void CollisionMap::Load(string file) {
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
                        int c = atoi(buff.c_str());
                        j++;
                        collisionTilesMatrix.push_back(c);
                        buff = "";
                    }
                }
                if(!buff.empty()) collisionTilesMatrix.push_back(atoi(buff.c_str()));
            }
            if (!getline(f, line) && !line.empty()) {
                throw "Error in file format in " + file;
            }
        }
    } else {
        throw "Erro ao abrir o arquivo " + file;
    }
}

int &CollisionMap::At(int x, int y, int z) {
    int index = x + mapWidth*y + mapWidth*mapHeight*z;
    return collisionTilesMatrix[index];
}

bool CollisionMap::IsColliding(Collider& collider) {
    auto& colliderObject = collider.GetGameObject();
    auto angleOfCollider = (float)(colliderObject.angleDeg * M_PI/180);
    auto colliderBox = collider.box;

    int layer = colliderObject.GetLayer();

    auto begin = clock();

    // get the unrotated vertices of the collider box (the vertices from the Rect itself)
    Vec2 vertices[] = { Vec2( colliderBox.x, colliderBox.y),
                        Vec2( colliderBox.x + colliderBox.w, colliderBox.y ),
                        Vec2( colliderBox.x + colliderBox.w, colliderBox.y + colliderBox.h),
                        Vec2( colliderBox.x, colliderBox.y + colliderBox.h )
    };

    // get the rotated vertices of the box, according to angleOfCollider (angleDeg)
    for (auto& v : vertices) {
        v = (v - colliderBox.Center()).Rotate(angleOfCollider) + colliderBox.Center();
    }

    Vec2 edges[] = { vertices[1] - vertices[0], vertices[2] - vertices[1], vertices[3] - vertices[2], vertices[0] - vertices[3]};

    for(int i = 0; i < 4; i++){
        Vec2 pivot(0,0);
        auto edgeNormalized = edges[i].Normalize();
        auto edgeMagnitude = edges[i].Module();

        while(pivot.Module() < edgeMagnitude){
            auto pointOfAnalysis = vertices[i] + pivot;

            if(At((int)pointOfAnalysis.x/tileWidth, (int)pointOfAnalysis.y/tileHeight, layer) != 0){
                return true;
            }

            pivot += (edgeNormalized * COLLISION_VERIFICATION_STEP_SIZE);
        }
    }

    return false;
}

int CollisionMap::GetTileWidth() {
    return tileWidth;
}

int CollisionMap::GetTileHeight() {
    return tileHeight;
}

int CollisionMap::GetMapWidth() {
    return mapWidth;
}

int CollisionMap::GetMapHeight() {
    return mapHeight;
}

int CollisionMap::GetMapDepth() {
    return mapDepth;
}

