#include "Tile.h"

Tile::Tile(const Vector2Int& gridPosition, bool isBlockingFlag)
: mGridPosition(gridPosition)
, mIsBlockingFlag(isBlockingFlag)
, mIsVisitedFlag(false)
{}

Tile::~Tile(void)
{}
