#include "StaticGameEntity.h"

StaticGameEntity::StaticGameEntity(Vector2Int gridPosition, Engine::Texture texture)
: GameEntity(gridPosition, texture)
{}

StaticGameEntity::~StaticGameEntity(void)
{}
