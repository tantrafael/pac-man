#define GLM_FORCE_RADIANS

#include <map>
#include <string>

#include "Game.h"
#include "Intro.h"
#include "Play.h"
#include "GameOver.h"

static const float FRAME_RATE_SAMPLING_TIME_INTERVAL = 0.2f;

static const std::map<Game::GameState, Game::GameState> STATE_FLOW_CHART = {
    { Game::STATE_INTRO, Game::STATE_PLAY },
    { Game::STATE_PLAY, Game::STATE_GAME_OVER },
    { Game::STATE_GAME_OVER, Game::STATE_PLAY }
};

Game::Game()
: mEngine(new Engine("."))
, mFrameRate(0)
, mScore(0)
, mFrameRateSamplingTimer(0.0f) {
	EnterState(STATE_INTRO);
	mEngine->Start(*this);
}

Game::~Game() {
}

void Game::RenderBackground() {
    mEngine->Render(Engine::TEXTURE_BACKGROUND, 0, 0);
}

void Game::RenderScore() {
	std::string s = std::to_string(mScore);
	char const *pchar = s.c_str();
	mEngine->Write("Score:", 205.0f, 4.0f);
	mEngine->Write(pchar, 303.0f, 4.0f);
}

void Game::RenderLives() {
	std::string s = std::to_string(mLives);
	char const *pchar = s.c_str();
	mEngine->Write("Lives:", 453.0f, 4.0f);
	mEngine->Write(pchar, 553.0f, 4.0f);
}

void Game::RenderFrameRate() {
	std::string s = std::to_string(mFrameRate);
	char const *pchar = s.c_str();
	mEngine->Write("FPS:", 704.0f, 4.0f);
	mEngine->Write(pchar, 780.0f, 4.0f);
}

void Game::EnterState(const GameState state) {
    mGameState = state;

    switch (state) {
        case STATE_PLAY:
            mState.reset(new Play(
				mEngine,
				std::bind(&Game::ExitState, this),
				std::bind(&Game::SetScore, this, std::placeholders::_1),
				std::bind(&Game::SetLives, this, std::placeholders::_1)
			));
            break;
            
        case STATE_GAME_OVER:
            mState.reset(new GameOver(mEngine, std::bind(&Game::ExitState, this)));
            break;
            
        default:
            mState.reset(new Intro(mEngine, std::bind(&Game::ExitState, this)));
            break;
    }
}

void Game::ExitState() {
    GameState gameState = STATE_FLOW_CHART.at(mGameState);
    EnterState(gameState);
}

void Game::SetScore(const int score) {
    mScore = score;
}
                                  
void Game::SetLives(const int lives) {
	mLives = lives;
}

void Game::Update() {
    float deltaTime = mEngine->GetLastFrameSeconds();

    RenderBackground();
	RenderScore();
	RenderLives();
	RenderFrameRate();

    if (mState != nullptr) {
        mState->Update(deltaTime);
		mState->Render(mEngine);
	}

	if ((mFrameRateSamplingTimer >= FRAME_RATE_SAMPLING_TIME_INTERVAL) && (deltaTime > 0.0f)) {
		mFrameRate = static_cast<int>(1.0f / deltaTime);
		mFrameRateSamplingTimer = 0.0f;
	}

	mFrameRateSamplingTimer += deltaTime;
}
