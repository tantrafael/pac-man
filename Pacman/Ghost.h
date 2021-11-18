#ifndef GHOST_H
#define GHOST_H

#include <memory>
#include <list>
#include "MovableGameEntity.h"

class World;
class Tile;

class Ghost : public MovableGameEntity
{
public:
	enum Character
	{
		CHARACTER_BLINKY,
		CHARACTER_PINKY,
		CHARACTER_INKY,
		CHARACTER_CLYDE
	};

	Ghost(Character character, const Vector2Int& gridPosition);
	~Ghost(void);

	bool mIsClaimableFlag;
	bool mIsDeadFlag;

	void Respawn();
	void Die(std::shared_ptr<World> aWorld);
	void Update(float deltaTime, std::shared_ptr<World> world, const Vector2Int& avatarGridPosition);
	void Render(std::shared_ptr<Engine> engine);

protected:
	Character mCharacter;
	Vector2Int mStartPosition;
	Vector2Int mDesiredMovement;
	std::list<std::shared_ptr<Tile>> mPath;
};

#endif // GHOST_H