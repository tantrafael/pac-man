#pragma once

#include <memory>

#include "State.h"

class Intro : public State {
public:
    Intro(std::shared_ptr<Engine> engine, std::function<void()> exitCallback);
    ~Intro();

    void Update(const float deltaTime);
	void Render(std::shared_ptr<Engine> engine);

private:
    void HandleUserInput();
};
