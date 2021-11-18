#pragma once

#include <memory>
#include <list>

#include "State.h"
#include "Vector2Int.h"
#include "Engine.h"

class Avatar;
class Ghost;
class World;

class Play : public State {
public:
	enum Mode {
		MODE_NORMAL,
		MODE_GHOST_ENCOUNTER,
		MODE_GHOSTS_VULNERABLE
	};
	
	Play(
		std::shared_ptr<Engine> engine,
		std::function<void()> exitCallback,
		std::function<void(int)> scoreCallback,
		std::function<void(int)> livesCallback
	);

    ~Play();

	bool CheckEndGameCondition();
    void Update(const float deltaTime);
	void Render(std::shared_ptr<Engine> engine);

private:
    std::function<void(int)> mScoreCallback;
	std::function<void(int)> mLivesCallback;
	int mLives;
	int mScore;
	Mode mMode;
	float mModeTimer;
	Vector2Int mNextMovement;
	std::unique_ptr<Avatar> mAvatar;
	std::list<std::shared_ptr<Ghost>> mGhosts;
	std::shared_ptr<World> mWorld;

	void HandlePlayerInput();
	void MoveAvatar();
    void UpdateScore(int score);
};
