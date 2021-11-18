#include "World.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Tile.h"
#include "Dot.h"
#include "BigDot.h"

static const float DOT_INTERSECTION_RADIUS = 5.0f;

World::World(const Vector2Int& dimensions)
: mGridDimensions(dimensions)
, mTiles(Matrix<std::shared_ptr<Tile>>(dimensions.y, MatrixRow<std::shared_ptr<Tile>>(dimensions.x)))
{
	Initialize();
}

World::~World(void)
{}

bool World::ValidGridPosition(const Vector2Int& gridPosition) {
	int rows = mGridDimensions.y;
	int cols = mGridDimensions.x;

	return (
		(gridPosition.x >= 0)
		&& (gridPosition.x < cols)
		&& (gridPosition.y >= 0)
		&& (gridPosition.y < rows)
	);
}

bool World::ValidTile(const Vector2Int& gridPosition)
{
	std::shared_ptr<Tile> tile = GetTile(gridPosition);

	return ((tile != nullptr) && !tile->mIsBlockingFlag);
}

bool World::HasIntersectedDot(const Vector2& position)
{
	for (auto& dot : mDots)
	{
		if ((dot->GetPosition() - position).Magnitude() < DOT_INTERSECTION_RADIUS)
		{
			mDots.remove(dot);
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2& position)
{
	for (auto& bigDot : mBigDots)
	{
		if ((bigDot->GetPosition() - position).Magnitude() < DOT_INTERSECTION_RADIUS)
		{
			mBigDots.remove(bigDot);
			return true;
		}
	}

	return false;
}

/*
bool World::HasIntersectedCherry(const Vector2& position)
{
	return true;
}
*/

void World::GetPath(
	const Vector2Int& startGridPosition,
	const Vector2Int& endGridPosition,
	std::list<std::shared_ptr<Tile>>& path
)
{
	std::shared_ptr<Tile> startTile = GetTile(startGridPosition);
	std::shared_ptr<Tile> endTile = GetTile(endGridPosition);

	for (int row = 0; row < mGridDimensions.y; ++row)
	{
		for (int col = 0; col < mGridDimensions.x; ++col)
		{
			Vector2Int gridPosition = Vector2Int(col, row);
			std::shared_ptr<Tile> tile = GetTile(gridPosition);
			tile->mIsVisitedFlag = false;
		}
	}

	Pathfind(startTile, endTile, path);
}

void World::Render(std::shared_ptr<Engine> engine)
{
	for (auto& dot : mDots)
	{
		dot->Render(engine);
	}

	for (auto& bigDot : mBigDots)
	{
		bigDot->Render(engine);
	}
}

void World::Initialize()
{
	std::string line;
	std::ifstream myfile("map.txt");

	if (myfile.is_open())
	{
		int lineIndex = 0;

		while (!myfile.eof())
		{
			std::getline(myfile, line);

			for (unsigned int i = 0; i < line.length(); ++i)
			{
				const Vector2Int gridPosition = Vector2Int(i, lineIndex);
				bool blocked = (line[i] == 'x');
				std::shared_ptr<Tile> tile = std::make_shared<Tile>(gridPosition, blocked);
				SetTile(gridPosition, tile);

				if (line[i] == '.')
				{
					std::shared_ptr<Dot> dot = std::make_shared<Dot>(gridPosition);
					mDots.push_back(dot);
				}
				else if (line[i] == 'o')
				{
					std::shared_ptr<BigDot> bigDot = std::make_shared<BigDot>(gridPosition);
					mBigDots.push_back(bigDot);
				}
			}

			lineIndex++;
		}

		myfile.close();
	}
}

bool World::Pathfind(
	std::shared_ptr<Tile>& startTile,
	std::shared_ptr<Tile>& endTile,
	std::list<std::shared_ptr<Tile>>& path
)
{
	startTile->mIsVisitedFlag = true;

	if (startTile->mIsBlockingFlag)
	{
		return false;
	}

	if (startTile == endTile)
	{
		return true;
	}

	std::list<std::shared_ptr<Tile>> validNeighborTiles;

	std::list<Vector2Int> neighborDirections = {
		Vector2Int::Left,
		Vector2Int::Right,
		Vector2Int::Down,
		Vector2Int::Up
	};

	for (auto& neighborDirection : neighborDirections)
	{
		Vector2Int neighborGridPosition = startTile->mGridPosition + neighborDirection;
		std::shared_ptr<Tile> neighborTile = GetTile(neighborGridPosition);

		if (neighborTile)
		{
			auto it = std::find(path.begin(), path.end(), neighborTile);
			bool inPath = (it != path.end());
			bool visited = neighborTile->mIsVisitedFlag;
			bool blocking = neighborTile->mIsBlockingFlag;

			if (!visited && !blocking && !inPath)
			{
				validNeighborTiles.push_front(neighborTile);
			}
		}
	}

	Vector2Int endGridPosition = endTile->mGridPosition;

	auto SortFromEndGridPosition = [endGridPosition](std::shared_ptr<Tile> a, std::shared_ptr<Tile> b) -> bool
	{
		Vector2Int da = a->mGridPosition - endGridPosition;
		Vector2Int db = b->mGridPosition - endGridPosition;
		//int la = abs(da.x) + abs(da.y);
		//int lb = abs(db.x) + abs(db.y);
		float ma = da.SqrMagnitude();
		float mb = db.SqrMagnitude();

		//return (la < lb);
		return (ma < mb);
	};

	validNeighborTiles.sort(SortFromEndGridPosition);

	for (auto validNeighborTile : validNeighborTiles)
	{
		path.push_back(validNeighborTile);

		if (Pathfind(validNeighborTile, endTile, path))
		{
			return true;
		}

		path.pop_back();
	}

	return false;
}

void World::SetTile(const Vector2Int& gridPosition, std::shared_ptr<Tile> tile)
{
	if (ValidGridPosition(gridPosition)) {
		mTiles[gridPosition.y][gridPosition.x] = tile;
	}
}

std::shared_ptr<Tile> World::GetTile(const Vector2Int& gridPosition)
{
	std::shared_ptr<Tile> tile = nullptr;

	if (ValidGridPosition(gridPosition)) {
		tile = mTiles[gridPosition.y][gridPosition.x];
	}

	return tile;
}
