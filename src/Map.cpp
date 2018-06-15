//
// Created by ftfnunes on 14/06/18.
//

#include <TileSet.h>
#include <TileMap.h>
#include <Map.h>

Map::Map(string mapName, string tileSetFile, Map::MapDirection direction) : direction(direction) {
    auto obj = new GameObject();
    auto tileSet = new TileSet(TILE_DIMENSIONS, TILE_DIMENSIONS, tileSetFile);
    auto map = new TileMap(*obj, mapName, tileSet);
    obj->AddComponent(map);

    tileMap = shared_ptr<GameObject>(obj);
}

shared_ptr<GameObject> Map::GetTileMap() {
    return tileMap;
}

void Map::SetPosition(Vec2 position) {
    tileMap->box.x = position.x;
    tileMap->box.y = position.y;
}

Map::MapDirection Map::GetDirection() {
    return direction;
}
