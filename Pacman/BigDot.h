#ifndef BIGDOT_H
#define BIGDOT_H

#include "StaticGameEntity.h"
#include "Vector2Int.h"

class BigDot : public StaticGameEntity
{
public:
	BigDot(Vector2Int gridPosition);
	~BigDot(void);
};

#endif // BIGDOT_H