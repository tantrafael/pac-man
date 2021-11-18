#pragma once

struct Input;

class Updater {
public:
	virtual void Update() = 0;
protected:
	~Updater() {}
};
