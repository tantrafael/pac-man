#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2Int.h"

class Dot : public StaticGameEntity
{
public:
	Dot(Vector2Int gridPosition);
	~Dot(void);
};

#endif // DOT_H