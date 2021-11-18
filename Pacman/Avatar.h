#ifndef AVATAR_H
#define AVATAR_H

#include <array>

#include "MovableGameEntity.h"

class Avatar : public MovableGameEntity
{
public:
	enum Orientation {
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT,
		ORIENTATION_DOWN,
		ORIENTATION_UP
	};

	enum Appearance {
		APPEARANCE_OPEN,
		APPEARANCE_CLOSED
	};

	Avatar(const Vector2Int& gridPosition);
	~Avatar(void);

	void Update(float deltaTime);

private:
	Orientation mOrientation;
	Appearance mAppearance;
	float mAppearanceTimer;
};

#endif //AVATAR_H