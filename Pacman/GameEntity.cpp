#include "GameEntity.h"
#include "Engine.h"
#include "Vector2Int.h"

static const int TILE_SIZE = 22;
static const Vector2Int RENDER_REFERENCE_POINT = Vector2Int(220, 60);

GameEntity::GameEntity(const Vector2Int& gridPosition, Engine::Texture texture)
: mPosition(gridPosition * TILE_SIZE)
, mTexture(texture)
, mIdMarkedForDeleteFlag(false)
{}

GameEntity::~GameEntity(void)
{}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}

void GameEntity::Render(std::shared_ptr<Engine> engine)
{
	engine->Render(
		mTexture,
		RENDER_REFERENCE_POINT.x + mPosition.x,
		RENDER_REFERENCE_POINT.y + mPosition.y
	);
}
