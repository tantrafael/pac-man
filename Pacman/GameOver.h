#pragma once

#include <memory>

#include "State.h"

class GameOver : public State {
public:
    GameOver(std::shared_ptr<Engine> engine, std::function<void()> exitCallback);
    ~GameOver();

    void Update(const float deltaTime);
	void Render(std::shared_ptr<Engine> engine);

private:
	bool mInputGiven;
	void HandleUserInput();
};
