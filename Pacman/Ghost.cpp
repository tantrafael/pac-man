#include "Ghost.h"

#include <map>

#include "World.h"
#include "Tile.h"
#include "Engine.h"

static const Vector2Int RESPAWN_POSITION = Vector2Int(13, 13);
static const float SPEED_NORMAL = 100.0f;
static const float SPEED_DEAD = 300.0f;
static const int TILE_SIZE = 22;
static const Vector2Int RENDER_REFERENCE_POINT = Vector2Int(220, 60);

static const std::map<Ghost::Character, Engine::Texture> TEXTURE_MAP = {
	{ Ghost::CHARACTER_BLINKY, Engine::TEXTURE_GHOST_BLINKY },
	{ Ghost::CHARACTER_PINKY, Engine::TEXTURE_GHOST_PINKY },
	{ Ghost::CHARACTER_INKY, Engine::TEXTURE_GHOST_INKY },
	{ Ghost::CHARACTER_CLYDE, Engine::TEXTURE_GHOST_CLYDE }
};

Ghost::Ghost(Character character, const Vector2Int& gridPosition)
: MovableGameEntity(gridPosition, TEXTURE_MAP.at(character))
{
	mStartPosition = gridPosition;
	mIsClaimableFlag = false;
	mIsDeadFlag = false;
	mDesiredMovement = Vector2Int::Zero;
}

Ghost::~Ghost(void)
{}

void Ghost::Respawn()
{
	mPath.clear();
	MovableGameEntity::Respawn();
}

void Ghost::Die(std::shared_ptr<World> world)
{
	mIsDeadFlag = true;
	mPath.clear();
	world->GetPath(mCurrentGridPosition, RESPAWN_POSITION, mPath);
}

void Ghost::Update(float deltaTime, std::shared_ptr<World> world, const Vector2Int& avatarGridPosition)
{
	if (IsAtDestination())
	{
		if (!mPath.empty())
		{
			std::shared_ptr<Tile> nextPathTile = mPath.front();
			mPath.pop_front();
			SetDestinationGridPosition(nextPathTile->mGridPosition);
		}
		else
		{
			if (mIsDeadFlag && (mCurrentGridPosition == RESPAWN_POSITION))
			{
				mIsDeadFlag = false;
				mIsClaimableFlag = false;
			}

			mPath.clear();
			world->GetPath(mCurrentGridPosition, avatarGridPosition, mPath);
		}
	}

	Vector2 destination = mDestinationGridPosition * TILE_SIZE;
	Vector2 direction = destination - mPosition;
	float speed = mIsDeadFlag ? SPEED_DEAD : SPEED_NORMAL;
	float distanceToMove = speed * deltaTime;

	if (distanceToMove > direction.Magnitude())
	{
		mPosition = destination;
		mCurrentGridPosition = mDestinationGridPosition;
	}
	else
	{
		direction.Normalize();
		mPosition += direction * distanceToMove;
	}
}

void Ghost::Render(std::shared_ptr<Engine> engine)
{
	Engine::Texture texture;

	if (mIsDeadFlag)
	{
		texture = Engine::TEXTURE_GHOST_DEAD;
	}
	else if (mIsClaimableFlag)
	{
		texture = Engine::TEXTURE_GHOST_VULNERABLE;
	}
	else
	{
		texture = mTexture;
	}

	engine->Render(
		texture,
		RENDER_REFERENCE_POINT.x + mPosition.x,
		RENDER_REFERENCE_POINT.y + mPosition.y
	);
}
