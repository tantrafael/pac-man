#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2Int.h"
#include "Engine.h"

class GameEntity
{
public:
	GameEntity(const Vector2Int& gridPosition, Engine::Texture texture);
	~GameEntity(void);

	Vector2 GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& position) { mPosition = position; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Render(std::shared_ptr<Engine> engine);

	void MarkForDelete() { mIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return mIdMarkedForDeleteFlag; }

protected:
	bool mIdMarkedForDeleteFlag;
	Vector2 mPosition;
	Engine::Texture mTexture;
};

#endif // GAMEENTITY_H