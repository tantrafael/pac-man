#ifndef CHERRY_H
#define CHERRY_H

#include "StaticGameEntity.h"
#include "Vector2Int.h"

class Cherry : public StaticGameEntity
{
public:
	Cherry(Vector2Int gridPosition);
	~Cherry(void);
};

#endif // CHERRY_H