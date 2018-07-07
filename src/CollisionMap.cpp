//
// Created by bruno on 22/05/18.
//

#include <Game.h>
#include <fstream>
#include <algorithm>
#include <Camera.h>
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
#ifdef DEBUG
    //Showing always the first layer (just for testing)
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            if (At(j, i) == 1) {
                continue;
            }
            auto box = Rect(associated.box.x + j*GetTileWidth(), associated.box.y + i*GetTileHeight(), GetTileHeight(), GetTileHeight());
            Vec2 center( box.Center() );
            SDL_Point points[5];

            Vec2 point = (Vec2(box.x, box.y) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
            point = Camera::GetRenderPosition(associated.GetLayer(), point);
            points[0] = {(int)point.x, (int)point.y};
            points[4] = {(int)point.x, (int)point.y};

            point = (Vec2(box.x + box.w, box.y) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
            point = Camera::GetRenderPosition(associated.GetLayer(), point);
            points[1] = {(int)point.x, (int)point.y};

            point = (Vec2(box.x + box.w, box.y + box.h) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
            point = Camera::GetRenderPosition(associated.GetLayer(), point);
            points[2] = {(int)point.x, (int)point.y};

            point = (Vec2(box.x, box.y + box.h) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
            point = Camera::GetRenderPosition(associated.GetLayer(), point);
            points[3] = {(int)point.x, (int)point.y};

            SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
            #endif
        }
    }
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

    int layer = colliderObject.GetLayer();

    // get the rotated vertices of the collider box (the vertices from the Rect itself)
    vector<Vec2> colliderVertices = collider.box.GetCorners(colliderObject.angleDeg, colliderObject.rotationCenter);

    float minX = numeric_limits<float>::infinity(), maxX = -numeric_limits<float>::infinity(), minY = numeric_limits<float>::infinity(), maxY = -numeric_limits<float>::infinity();

    // get the min and max of collider vertices
    for (auto& v : colliderVertices) {
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
        auto box = associated.box;

        if (maxY < box.y || minY > box.y + box.h || maxX < box.x || minX > box.x + box.w) {
            return false;
        }
        minX -= box.x;
        minY -= box.y;
        maxX -= box.x;
        maxY -= box.y;

        int tilesMinCol = minX > 0 ? (int)(minX)/tileWidth : 0;
        int tilesMaxCol = maxX < box.w ? (int)(maxX)/tileWidth : mapWidth-1;
        int tilesMinRow = minY > 0 ? (int)(minY)/tileHeight : 0;
        int tilesMaxRow = maxY < box.h ? (int)(maxY)/tileHeight : mapHeight-1;

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

                if(At(tileCol, tileRow, layer) == 1)
                    continue; // we just want to check the non-walkable tiles

                int tileMinX = box.x + tileCol * tileWidth;
                int tileMaxX = tileMinX + tileWidth;
                int tileMinY = box.y + tileRow * tileHeight;
                int tileMaxY = tileMinY + tileHeight;

                Vec2 tileVertices[] = { Vec2( tileMinX, tileMaxY ),
                                        Vec2( tileMaxX, tileMaxY ),
                                        Vec2( tileMaxX, tileMinY ),
                                        Vec2( tileMinX, tileMinY )
                };

                auto tileNotColliding = false;
                for (int axis = 0; axis < 4; axis++) {
                    float P[4];

                    for (int vertice = 0; vertice < 4; vertice++) P[vertice] = tileVertices[vertice] * axes[axis];

                    float minTile = *std::min_element(P, P + 4);
                    float maxTile = *std::max_element(P, P + 4);

                    if (maxCollider[axis] < minTile || minCollider[axis] > maxTile) {
                        tileNotColliding = true;
                        break; // not colliding with this tile. Lets try with another one...
                    }
                }
                
                if (tileNotColliding) {
                    continue;
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

vector<Intersection> CollisionMap::GetIntersections(Collider &collider) {
    vector<Intersection> intersections;
    auto &colliderObject = collider.GetGameObject();
    auto layer = colliderObject.GetLayer();
    // get the rotated vertices of the collider box (the vertices from the Rect itself)
    vector<Vec2> colliderVertices = collider.box.GetCorners(colliderObject.angleDeg, colliderObject.rotationCenter);

    float minX = numeric_limits<float>::infinity(), maxX = -numeric_limits<float>::infinity(), minY = numeric_limits<float>::infinity(), maxY = -numeric_limits<float>::infinity();

    // get the min and max of collider vertices
    for (auto& v : colliderVertices) {
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

    auto box = associated.box;

    if (maxY < box.y || minY > box.y + box.h || maxX < box.x || minX > box.x + box.w) {
        return intersections;
    }
    minX -= box.x;
    minY -= box.y;
    maxX -= box.x;
    maxY -= box.y;

    int tilesMinCol = minX > 0 ? (int)(minX)/tileWidth : 0;
    int tilesMaxCol = maxX < box.w ? (int)(maxX)/tileWidth : mapWidth-1;
    int tilesMinRow = minY > 0 ? (int)(minY)/tileHeight : 0;
    int tilesMaxRow = maxY < box.h ? (int)(maxY)/tileHeight : mapHeight-1;

    for(int tileRow = tilesMinRow; tileRow <= tilesMaxRow; tileRow++){
        for(int tileCol = tilesMinCol; tileCol <= tilesMaxCol; tileCol++) {

            if (At(tileCol, tileRow, layer) == 1)
                continue; // we just want to check the non-walkable tiles

            vector<LineSegment> colliderLines;
            vector<LineSegment> collidableLines;

            auto colliderCorners = collider.box.GetCorners(colliderObject.angleDeg, colliderObject.rotationCenter);
            int tileMinX = box.x + tileCol * tileWidth;
            int tileMaxX = tileMinX + tileWidth;
            int tileMinY = box.y + tileRow * tileHeight;
            int tileMaxY = tileMinY + tileHeight;

            vector<Vec2> collidableCorners = {
                Vec2( tileMinX, tileMaxY ),
                Vec2( tileMaxX, tileMaxY ),
                Vec2( tileMaxX, tileMinY ),
                Vec2( tileMinX, tileMinY )
            };

            for (int i = 0; i < 4; ++i) {
                auto next = (i+1)%4;
                colliderLines.emplace_back(colliderCorners[i], colliderCorners[next]);
                collidableLines.emplace_back(collidableCorners[i], collidableCorners[next]);
            }

            for (auto &colliderLine : colliderLines) {
                for (auto &collidableLine : collidableLines) {
                    if (colliderLine == collidableLine) {
                        intersections.emplace_back(colliderLine, collidableLine, (colliderLine.dot1 - colliderLine.dot2)*0.5);
                    } else {
                        auto intersection = colliderLine.GetIntersection(collidableLine);
                        if (collidableLine.Contains(intersection) && colliderLine.Contains(intersection)) {
                            intersections.emplace_back(colliderLine, collidableLine, intersection);
                        }
                    }
                }
            }
        }
    }

    return intersections;
}
