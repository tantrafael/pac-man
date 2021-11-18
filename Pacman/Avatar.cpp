#include "Avatar.h"

#include <map>

#include "Vector2Int.h"
#include "Engine.h"

static const int TILE_SIZE = 22;
static const float SPEED = 100.0f;
static const float APPEARANCE_TIME = 0.15f;
static const Vector2Int RENDER_REFERENCE_POINT = Vector2Int(220, 60);

static const std::map<std::pair<Avatar::Orientation, Avatar::Appearance>, Engine::Texture> TEXTURE_MAP = {
	{ std::make_pair(Avatar::ORIENTATION_LEFT, Avatar::APPEARANCE_OPEN), Engine::TEXTURE_AVATAR_LEFT_OPEN },
	{ std::make_pair(Avatar::ORIENTATION_LEFT, Avatar::APPEARANCE_CLOSED), Engine::TEXTURE_AVATAR_LEFT_CLOSED },
	{ std::make_pair(Avatar::ORIENTATION_RIGHT, Avatar::APPEARANCE_OPEN), Engine::TEXTURE_AVATAR_RIGHT_OPEN },
	{ std::make_pair(Avatar::ORIENTATION_RIGHT, Avatar::APPEARANCE_CLOSED), Engine::TEXTURE_AVATAR_RIGHT_CLOSED },
	{ std::make_pair(Avatar::ORIENTATION_DOWN, Avatar::APPEARANCE_OPEN), Engine::TEXTURE_AVATAR_DOWN_OPEN },
	{ std::make_pair(Avatar::ORIENTATION_DOWN, Avatar::APPEARANCE_CLOSED), Engine::TEXTURE_AVATAR_DOWN_CLOSED },
	{ std::make_pair(Avatar::ORIENTATION_UP, Avatar::APPEARANCE_OPEN), Engine::TEXTURE_AVATAR_UP_OPEN },
	{ std::make_pair(Avatar::ORIENTATION_UP, Avatar::APPEARANCE_CLOSED), Engine::TEXTURE_AVATAR_UP_CLOSED }
};

Avatar::Avatar(const Vector2Int& gridPosition)
: MovableGameEntity(gridPosition, Engine::TEXTURE_AVATAR_LEFT_OPEN)
, mOrientation(ORIENTATION_LEFT)
, mAppearance(APPEARANCE_OPEN)
, mAppearanceTimer(APPEARANCE_TIME)
{}

Avatar::~Avatar(void)
{}

void Avatar::Update(float deltaTime)
{
	Vector2 destination = mDestinationGridPosition * TILE_SIZE;
	Vector2 direction = destination - mPosition;
	float distanceToMove = SPEED * deltaTime;

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

	Avatar::Orientation orientation = mOrientation;
	Avatar::Appearance appearance = mAppearance;

	if (direction != Vector2::Zero)
	{
		if (direction.x != 0.0f)
		{
			orientation = (direction.x < 0) ? ORIENTATION_LEFT : ORIENTATION_RIGHT;
		}
		else
		{
			orientation = (direction.y > 0) ? ORIENTATION_DOWN : ORIENTATION_UP;
		}

		mAppearanceTimer -= deltaTime;

		if (mAppearanceTimer <= 0.0f)
		{
			appearance = (mAppearance == APPEARANCE_OPEN) ? APPEARANCE_CLOSED : APPEARANCE_OPEN;
			mAppearanceTimer = APPEARANCE_TIME;
		}
	}
	else
	{
		if (mAppearance == APPEARANCE_CLOSED)
		{
			appearance = APPEARANCE_OPEN;
		}

		mAppearanceTimer = APPEARANCE_TIME;
	}

	if ((mOrientation != orientation) || (mAppearance != appearance))
	{
		mOrientation = orientation;
		mAppearance = appearance;
		mTexture = TEXTURE_MAP.at(std::make_pair(mOrientation, mAppearance));
	}
}
