//
// Created by bruno on 22/05/18.
//

#ifndef NEPHESH_COLLISIONMAP_H
#define NEPHESH_COLLISIONMAP_H

#include <vector>
#include "Collidable.h"

#define COLLISION_MAP_TYPE "CollisionMap"
#define COLLISION_VERIFICATION_STEP_SIZE 2

class CollisionMap : public Collidable {
public:
    CollisionMap(GameObject &associated, string file, int tileWidth, int tileHeight);

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void Load(string file);
    int At(int x, int y, int z = 0);

    bool IsColliding(Collider& collider) override;

    int GetTileWidth();
    int GetTileHeight();
    int GetMapWidth();
    int GetMapHeight();
    int GetMapDepth();

    vector<Intersection> GetIntersections(Collider &collider) override;

private:
    vector<int> collisionTilesMatrix;
    int tileWidth;
    int tileHeight;
    int mapWidth;
    int mapHeight;
    int mapDepth;
};


#endif //NEPHESH_COLLISIONMAP_H
