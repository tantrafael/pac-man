#include "State.h"

State::State(std::shared_ptr<Engine> engine, std::function<void()> exitCallback)
: mEngine(engine)
, mExitCallback(exitCallback) {
}

State::~State() {
}
