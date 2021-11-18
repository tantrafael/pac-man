#ifndef STATICGAMEENTITY_H
#define STATICGAMEENTITY_H

#include "GameEntity.h"

class StaticGameEntity : public GameEntity
{
public:
	StaticGameEntity(Vector2Int gridPosition, Engine::Texture texture);
	~StaticGameEntity(void);
};

#endif // STATICGAMEENTITY_H