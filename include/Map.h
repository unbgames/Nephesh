//
// Created by ftfnunes on 14/06/18.
//

#ifndef NEPHESH_MAP_H
#define NEPHESH_MAP_H

#include <iostream>
#include <memory>

using namespace std;

class Map {
public:
    enum MapDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Map(string mapName, string tileSetFile, MapDirection direction);

    MapDirection GetDirection();
    void SetPosition(Vec2 position);
    shared_ptr<GameObject> GetTileMap();
private:
    MapDirection direction;
    shared_ptr<GameObject> tileMap;
};

#endif //NEPHESH_MAP_H