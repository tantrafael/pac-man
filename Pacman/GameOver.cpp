#include "GameOver.h"

GameOver::GameOver(std::shared_ptr<Engine> engine, std::function<void()> exitCallback)
: State(engine, exitCallback)
, mInputGiven(mEngine->GetMouseButtonDown() || (mEngine->GetCurrentKeyCode() >= 0))
{}

GameOver::~GameOver() {
}

void GameOver::Update(const float deltaTime) {
	HandleUserInput();
}

void GameOver::Render(std::shared_ptr<Engine> engine)
{
	mEngine->Write("Game over", 424.0f, 320.0f);
	mEngine->Write("Play again", 424.0f, 360.0f);
}

void GameOver::HandleUserInput() {
	if (mEngine->GetMouseButtonDown() || (mEngine->GetCurrentKeyCode() >= 0))
	{
		if (!mInputGiven) {
			mExitCallback();
		}
	}
	else
	{
		mInputGiven = false;
	}
}
