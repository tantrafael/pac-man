#pragma once

#include <memory>

#include <SDL.h>

class SdlWindow;

class GlContext {
public:
	GlContext(SdlWindow& sdlWindow);
	operator SDL_GLContext();
private:
	std::unique_ptr<void, void(*)(SDL_GLContext)> mContext;
};
