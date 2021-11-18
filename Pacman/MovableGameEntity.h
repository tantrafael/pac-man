#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"

#include "Vector2Int.h"

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2Int& gridPosition, Engine::Texture texture);
	~MovableGameEntity(void);
	void SetDestinationGridPosition(const Vector2Int& gridPosition);
	Vector2Int GetCurrentGridPosition() const;
	bool IsAtDestination();
	void Respawn();

protected:
	Vector2Int mStartGridPosition;
	Vector2Int mCurrentGridPosition;
	Vector2Int mDestinationGridPosition;
};

#endif // MOVABLEGAMEENTITY_H