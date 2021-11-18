#pragma once

#include <memory>
#include <functional>

#include "Engine.h"

class State {
public:
    State(std::shared_ptr<Engine> engine, std::function<void()> exitCallback);
    ~State();

    virtual void Update(const float deltaTime) = 0;
	virtual void Render(std::shared_ptr<Engine> engine) = 0;

protected:
    std::shared_ptr<Engine> mEngine;
    std::function<void()> mExitCallback;
};
