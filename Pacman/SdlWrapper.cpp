#include "SdlWrapper.h"

#include <SDL.h>
#include <stdexcept>

SdlWrapper::SdlWrapper(int flags) {
	if (SDL_Init(flags) != 0) {
		throw std::runtime_error("Failed to init SDL");
	}
}
SdlWrapper::~SdlWrapper() {
	SDL_Quit();
}
