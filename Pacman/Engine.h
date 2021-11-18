#pragma once

#include "glm/fwd.hpp"
#include <memory>
#include <stdint.h>

class Updater;

class Engine {
public:
    enum Texture {
        TEXTURE_BACKGROUND,
		TEXTURE_AVATAR_LEFT_OPEN,
		TEXTURE_AVATAR_LEFT_CLOSED,
		TEXTURE_AVATAR_RIGHT_OPEN,
		TEXTURE_AVATAR_RIGHT_CLOSED,
		TEXTURE_AVATAR_DOWN_OPEN,
		TEXTURE_AVATAR_DOWN_CLOSED,
		TEXTURE_AVATAR_UP_OPEN,
		TEXTURE_AVATAR_UP_CLOSED,
		TEXTURE_GHOST_BLINKY,
		TEXTURE_GHOST_PINKY,
		TEXTURE_GHOST_INKY,
		TEXTURE_GHOST_CLYDE,
		TEXTURE_GHOST_VULNERABLE,
		TEXTURE_GHOST_DEAD,
		TEXTURE_DOT,
		TEXTURE_BIG_DOT,
		TEXTURE_CHERRY,
		TEXTURE_MAX
    };

    Engine(const char* assetsDirectory);
    ~Engine();
        
    float GetLastFrameSeconds() const;
    float GetMouseX() const;
    float GetMouseY() const;
    bool GetMouseButtonDown() const;
	int GetCurrentKeyCode() const;

    void Start(Updater& updater);
    void Quit();
        
    int GetTextureHeight(Texture texture) const;
    int GetTextureWidth(Texture texture) const;
    void Render(Texture texture, const glm::mat4& transform);
    void Render(Texture texture, float x, float y, float rotation = 0.0f);

    float CalculateStringWidth(const char* text) const;
    void Write(const char* text, const glm::mat4& transform);
    void Write(const char* text, float x, float y, float rotation = 0.0f);
        
    int GetWidth() const;
    int GetHeight() const;
        
private:
    struct EngineImplementation;
    std::unique_ptr<EngineImplementation> mPimpl;
};
