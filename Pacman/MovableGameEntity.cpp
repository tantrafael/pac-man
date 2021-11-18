#include "MovableGameEntity.h"

#include <math.h>

const int TILE_SIZE = 22;

MovableGameEntity::MovableGameEntity(const Vector2Int& gridPosition, Engine::Texture texture)
: GameEntity(gridPosition, texture)
{
	mStartGridPosition = gridPosition;
	mCurrentGridPosition.x = mDestinationGridPosition.x = static_cast<int>(floorf(mPosition.x / TILE_SIZE));
	mCurrentGridPosition.y = mDestinationGridPosition.y = static_cast<int>(floorf(mPosition.y / TILE_SIZE));
}

MovableGameEntity::~MovableGameEntity(void)
{}

bool MovableGameEntity::IsAtDestination()
{
	return (mCurrentGridPosition == mDestinationGridPosition);
}

void MovableGameEntity::SetDestinationGridPosition(const Vector2Int& gridPosition)
{
	mDestinationGridPosition = gridPosition;
}

Vector2Int MovableGameEntity::GetCurrentGridPosition() const
{
	return mCurrentGridPosition;
}

void MovableGameEntity::Respawn()
{
	mCurrentGridPosition = mDestinationGridPosition = mStartGridPosition;
	mPosition = mCurrentGridPosition * TILE_SIZE;
}
