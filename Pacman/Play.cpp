#include "Play.h"

#include <math.h>
#include <array>
#include <vector>
#include <map>

#include "Vector2Int.h"
#include "Vector2.h"
#include "State.h"
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"

static const Vector2Int WORLD_GRID_DIMENSIONS = Vector2Int(26, 29);
static const Vector2Int AVATAR_START_POSITION = Vector2Int(13, 22);
static const float GHOST_INTERSECTION_RADIUS = 10.0f;
static const int SCORE_DOT = 10;
static const int SCORE_BIG_DOT = 20;
static const int SCORE_GHOST = 50;

static const std::map<Ghost::Character, Vector2Int> GHOST_START_POSITIONS = {
	{ Ghost::CHARACTER_BLINKY, Vector2Int(11, 13) },
	{ Ghost::CHARACTER_PINKY, Vector2Int(13, 13) },
	{ Ghost::CHARACTER_INKY, Vector2Int(15, 13) },
	{ Ghost::CHARACTER_CLYDE, Vector2Int(13, 10) }
};

static const float GHOST_ENCOUNTER_TIME = 1.0f;
static const float GHOSTS_VULNERABLE_TIME = 20.0f;

Play::Play(
	std::shared_ptr<Engine> engine,
	std::function<void()> exitCallback,
	std::function<void(int)> scoreCallback,
	std::function<void(int)> livesCallback
)
: State(engine, exitCallback)
, mScoreCallback(scoreCallback)
, mLivesCallback(livesCallback)
, mLives(3)
, mScore(0)
, mModeTimer(0.0f)
, mNextMovement(Vector2Int::Left)
, mMode(Mode::MODE_NORMAL)
{
	mAvatar = std::make_unique<Avatar>(AVATAR_START_POSITION);

	for (auto element : GHOST_START_POSITIONS)
	{
		Ghost::Character character = element.first;
		Vector2Int startGridPosition = element.second;
		std::shared_ptr<Ghost> ghost = std::make_shared<Ghost>(character, startGridPosition);
		mGhosts.push_back(ghost);
	}

	mWorld = std::make_unique<World>(WORLD_GRID_DIMENSIONS);

	mScoreCallback(mScore);
	mLivesCallback(mLives);
}

Play::~Play()
{}

bool Play::CheckEndGameCondition()
{
	return false;
}

void Play::Update(const float deltaTime)
{
	if ((mMode == Mode::MODE_NORMAL) || (mMode == Mode::MODE_GHOSTS_VULNERABLE))
	{
		HandlePlayerInput();
		MoveAvatar();
		mAvatar->Update(deltaTime);
		Vector2Int avatarGridPosition = mAvatar->GetCurrentGridPosition();

		for (auto& ghost : mGhosts)
		{
			ghost->Update(deltaTime, mWorld, avatarGridPosition);
			Vector2 deltaPosition = ghost->GetPosition() - mAvatar->GetPosition();
			float distance = deltaPosition.Magnitude();

			if (distance < GHOST_INTERSECTION_RADIUS)
			{
				if (mMode == Mode::MODE_NORMAL)
				{
					mLives--;
					if (mLives < 0) { mLives = 0; }
					mLivesCallback(mLives);

					mMode = MODE_GHOST_ENCOUNTER;
					mModeTimer = GHOST_ENCOUNTER_TIME;
				}
				else if ((mMode == Mode::MODE_GHOSTS_VULNERABLE) && ghost->mIsClaimableFlag && !ghost->mIsDeadFlag)
				{
					UpdateScore(SCORE_GHOST);
					ghost->mIsDeadFlag = true;
					ghost->Die(mWorld);
				}
			}
		}

		if (mWorld->HasIntersectedDot(mAvatar->GetPosition())) {
			UpdateScore(SCORE_DOT);
		}

		if (mWorld->HasIntersectedBigDot(mAvatar->GetPosition()))
		{
			UpdateScore(SCORE_BIG_DOT);
			mMode = Mode::MODE_GHOSTS_VULNERABLE;
			mModeTimer = GHOSTS_VULNERABLE_TIME;

			for (auto& ghost : mGhosts)
			{
				ghost->mIsClaimableFlag = true;
			}
		}

		if (mMode == Mode::MODE_GHOSTS_VULNERABLE)
		{
			mModeTimer -= deltaTime;

			if (mModeTimer <= 0.0f)
			{
				mMode = Mode::MODE_NORMAL;
				mModeTimer = 0.0f;

				for (auto& ghost : mGhosts)
				{
					ghost->mIsClaimableFlag = false;
				}
			}
		}
	}
	else if (mMode == Mode::MODE_GHOST_ENCOUNTER)
	{
		mModeTimer -= deltaTime;

		if (mModeTimer <= 0.0f)
		{
			if (mLives > 0)
			{
				mMode = Mode::MODE_NORMAL;
				mModeTimer = 0.0f;
				mAvatar->Respawn();

				for (auto& ghost : mGhosts)
				{
					ghost->Respawn();
				}
			}
			else
			{
				mExitCallback();
			}
		}
	}
}

void Play::Render(std::shared_ptr<Engine> engine)
{
	mWorld->Render(engine);
	mAvatar->Render(engine);

	for (auto& ghost : mGhosts)
	{
		ghost->Render(engine);
	}
}

void Play::HandlePlayerInput()
{
	int currentKeyCode = mEngine->GetCurrentKeyCode();

	if (currentKeyCode == 1)
	{
		mNextMovement = Vector2Int::Left;
	}
	else if (currentKeyCode == 2)
	{
		mNextMovement = Vector2Int::Right;
	}
	else if (currentKeyCode == 4)
	{
		mNextMovement = Vector2Int::Down;
	}
	else if (currentKeyCode == 3)
	{
		mNextMovement = Vector2Int::Up;
	}
	else
	{
		//mNextMovement = Vector2Int::Zero;
	}
}

void Play::MoveAvatar()
{
	Vector2Int destinationTile = mAvatar->GetCurrentGridPosition() + mNextMovement;

	if (mAvatar->IsAtDestination())
	{
		if (mWorld->ValidTile(destinationTile))
		{
			mAvatar->SetDestinationGridPosition(destinationTile);
		}
	}
}

void Play::UpdateScore(int score) {
	mScore += score;
	mScoreCallback(mScore);
}
