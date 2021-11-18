#include "Intro.h"

Intro::Intro(std::shared_ptr<Engine> engine, std::function<void()> exitCallback)
: State(engine, exitCallback)
{}

Intro::~Intro()
{}

void Intro::Update(const float deltaTime)
{
	HandleUserInput();
}

void Intro::Render(std::shared_ptr<Engine> engine)
{
	mEngine->Write("Play", 475.0f, 342.0f);
}

void Intro::HandleUserInput() {
    if (mEngine->GetMouseButtonDown() || (mEngine->GetCurrentKeyCode() >= 0)) {
        mExitCallback();
    }
}
