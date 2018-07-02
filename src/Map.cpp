//
// Created by ftfnunes on 14/06/18.
//

#include <TileSet.h>
#include <TileMap.h>
#include <Map.h>
#include <CollisionMap.h>
#include <Game.h>
#include <WorldState.h>

Map::Map(string mapName, string tileSetFile, Map::MapDirection direction, string collisionMapFile) : direction(direction) {
    auto obj = new GameObject();
    auto tileSet = new TileSet(TILE_DIMENSIONS, TILE_DIMENSIONS, tileSetFile);
    auto map = new TileMap(*obj, mapName, tileSet);
    obj->AddComponent(map);

    tileMap = shared_ptr<GameObject>(obj);

    if (!collisionMapFile.empty()) {
        tileMap->AddComponent(new CollisionMap(*tileMap, collisionMapFile, TILE_DIMENSIONS, TILE_DIMENSIONS));
        auto &state = (WorldState &) Game::GetInstance().GetCurrentState();
        state.AddCollidable(tileMap);
        cout << "added collisionMap" << endl;
    }
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
