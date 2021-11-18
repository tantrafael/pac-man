#ifndef TILE_H
#define TILE_H

#include "Vector2Int.h"

class Tile
{
public:
	Tile(const Vector2Int& gridPosition, bool isBlockingFlag);
	~Tile(void);

	Vector2Int mGridPosition;
	bool mIsBlockingFlag;
	bool mIsVisitedFlag;
};

#endif // TILE_H