#pragma once

#include <memory>

#include "Engine.h"
#include "Updater.h"

#include "State.h"

class Game : public Updater {
public:
    enum GameState {
        STATE_INTRO,
        STATE_PLAY,
        STATE_GAME_OVER
    };

    Game();
    ~Game();

private:
    std::shared_ptr<Engine> mEngine;
    GameState mGameState;
    std::unique_ptr<State> mState;
	int mFrameRate;
	int mLives;
	int mScore;
	float mFrameRateSamplingTimer;

    void RenderBackground();
	void RenderFrameRate();
	void RenderLives();
	void RenderScore();
    void EnterState(const GameState state);
    void ExitState();
    void SetScore(const int score);
	void SetLives(const int lives);
	void Update();
};
