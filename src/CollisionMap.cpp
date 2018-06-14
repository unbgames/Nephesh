//
// Created by bruno on 22/05/18.
//

#include <Game.h>
#include <fstream>
#include <algorithm>
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

int CollisionMap::At(int x, int y, int z) {
    // default behavior when the player is outside the mapped area
    if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || z < 0 || z >= mapDepth)
        return 0;

    int index = x + mapWidth*y + mapWidth*mapHeight*z;
    return collisionTilesMatrix[index];
}

bool CollisionMap::IsColliding(Collider& collider) {
    // 1 - SAT ON TILES ALGORITHM
    // 2 - EDGE SWEEP ALGORITHM
    const int ALGORITHM = 1;

    auto& colliderObject = collider.GetGameObject();
    auto angleOfCollider = (float)(colliderObject.angleDeg * M_PI/180);
    auto colliderBox = collider.box;

    int layer = colliderObject.GetLayer();

    // get the unrotated vertices of the collider box (the vertices from the Rect itself)
    Vec2 colliderVertices[] = { Vec2( colliderBox.x, colliderBox.y + colliderBox.h ),
                                Vec2( colliderBox.x + colliderBox.w, colliderBox.y + colliderBox.h ),
                                Vec2( colliderBox.x + colliderBox.w, colliderBox.y ),
                                Vec2( colliderBox.x, colliderBox.y )
    };


    float minX = numeric_limits<float>::infinity(), maxX = -numeric_limits<float>::infinity(), minY = numeric_limits<float>::infinity(), maxY = -numeric_limits<float>::infinity();

    // get the rotated vertices of the box, according to angleOfCollider (angleDeg)
    for (auto& v : colliderVertices) {
        v = (v - colliderBox.Center()).Rotate(angleOfCollider) + colliderBox.Center();

        // find the minima and the maxima of the coordinates x and y (the vertices of the AABB). This is used on SAT ON TILES ALGORITHM)
        if(v.x > maxX)
            maxX = v.x;
        if(v.x < minX)
            minX = v.x;
        if(v.y > maxY)
            maxY = v.y;
        if(v.y < minY)
            minY = v.y;
    }


    if(ALGORITHM == 1){
        // --------------------------------
        // -----SAT ON TILES ALGORITHM-----
        // --------------------------------

        int tilesMinCol = (int)minX/tileWidth;
        int tilesMaxCol = (int)maxX/tileWidth;
        int tilesMinRow = (int)minY/tileHeight;
        int tilesMaxRow = (int)maxY/tileHeight;

        // the axes [2] and [3] are the same for all the tiles, thats why they are hard-coded
        Vec2 axes[] = { (colliderVertices[0] - colliderVertices[1]).Normalize(),
                        (colliderVertices[1] - colliderVertices[2]).Normalize(),
                        Vec2(-1,0),
                        Vec2(0,1)
        };

        float minCollider[4] = {0}, maxCollider[4] = {0};

        for (int axis = 0; axis < 4; axis++) {
            float P[4];

            for (int vertice = 0; vertice < 4; vertice++) P[vertice] = colliderVertices[vertice] * axes[axis];

            minCollider[axis] = *std::min_element(P, P + 4);
            maxCollider[axis] = *std::max_element(P, P + 4);
        }

        // verify SAT collision on each of the non-walkable collision tiles inside the AABB
        for(int tileRow = tilesMinRow; tileRow <= tilesMaxRow; tileRow++){
            for(int tileCol = tilesMinCol; tileCol <= tilesMaxCol; tileCol++){

                if(At(tileCol, tileRow, layer) == 0)
                    continue; // we just want to check the non-walkable tiles

                int tileMinX = tileCol * tileWidth;
                int tileMaxX = tileMinX + tileWidth;
                int tileMinY = tileRow * tileHeight;
                int tileMaxY = tileMinY + tileHeight;

                Vec2 tileVertices[] = { Vec2( tileMinX, tileMaxY ),
                                        Vec2( tileMaxX, tileMaxY ),
                                        Vec2( tileMaxX, tileMinY ),
                                        Vec2( tileMinX, tileMinY )
                };

                for (int axis = 0; axis < 4; axis++) {
                    float P[4];

                    for (int vertice = 0; vertice < 4; vertice++) P[vertice] = tileVertices[vertice] * axes[axis];

                    float minTile = *std::min_element(P, P + 4);
                    float maxTile = *std::max_element(P, P + 4);

                    if (maxCollider[axis] < minTile || minCollider[axis] > maxTile)
                        continue; // not colliding with this tile. Lets try with another one...
                }

                return true; // collided with this tile...
            }
        }

        return false; // tested with all tiles, and there were no collisions...
    }


    else{
        // ------------------------------
        // -----EDGE SWEEP ALGORITHM-----
        // ------------------------------

        Vec2 edges[] = { colliderVertices[1] - colliderVertices[0], colliderVertices[2] - colliderVertices[1], colliderVertices[3] - colliderVertices[2], colliderVertices[0] - colliderVertices[3]};

        for(int i = 0; i < 4; i++){
            Vec2 pivot(0,0);
            auto edgeNormalized = edges[i].Normalize();
            auto edgeMagnitude = edges[i].Module();

            while(pivot.Module() < edgeMagnitude){
                auto pointOfAnalysis = colliderVertices[i] + pivot;

                if(At((int)pointOfAnalysis.x/tileWidth, (int)pointOfAnalysis.y/tileHeight, layer) != 0){
                    return true;
                }

                pivot += (edgeNormalized * COLLISION_VERIFICATION_STEP_SIZE);
            }
        }

        return false;
    }
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

