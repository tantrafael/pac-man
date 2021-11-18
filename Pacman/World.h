#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <list>
#include <vector>
#include "Vector2.h"
#include "Vector2Int.h"
#include "Engine.h"

class Tile;
class Dot;
class BigDot;
//class Cherry;

template <class T>
using MatrixRow = std::vector<T>;

template <class T>
using Matrix = std::vector<MatrixRow<T>>;

class World
{
public:
	World(const Vector2Int& dimensions);
	~World(void);
	bool ValidGridPosition(const Vector2Int& gridPosition);
	bool ValidTile(const Vector2Int& gridPosition);
	bool HasIntersectedDot(const Vector2& position);
	bool HasIntersectedBigDot(const Vector2& position);
	//bool HasIntersectedCherry(const Vector2& position);

	void GetPath(
		const Vector2Int& startGridPosition,
		const Vector2Int& endGridPosition,
		std::list<std::shared_ptr<Tile>>& path
	);

	void Render(std::shared_ptr<Engine> engine);

private:
	Vector2Int mGridDimensions;
	Matrix<std::shared_ptr<Tile>> mTiles;
	std::list<std::shared_ptr<Dot>> mDots;
	std::list<std::shared_ptr<BigDot>> mBigDots;
	//std::list<Cherry*> myCherry;

	void Initialize();
	void SetTile(const Vector2Int& gridPosition, std::shared_ptr<Tile> tile);
	std::shared_ptr<Tile> GetTile(const Vector2Int& gridPosition);

	bool Pathfind(
		std::shared_ptr<Tile>& startTile,
		std::shared_ptr<Tile>& endTile,
		std::list<std::shared_ptr<Tile>>& path
	);
};

#endif // WORLD_H
