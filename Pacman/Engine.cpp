#include "Engine.h"

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include "gl/glew.h"
#include "SDL_opengl.h"
#include "gl/glu.h"
#include <stdio.h>

#include "Font.h"
#include "GlContext.h"
#include "SdlWrapper.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "Updater.h"

static const int WindowWidth = 1024;
static const int WindowHeight = 768;
static const float MaxFrameTicks = 300.0f;
static const float TextScale = 0.5f;

struct Engine::EngineImplementation {
	SdlWrapper mSdlWrapper;
    SdlWindow mSdlWindow;
    GlContext mGlContext;
    std::unique_ptr<SdlSurface> mSdlSurfaceContainer[Engine::TEXTURE_MAX];
    std::unique_ptr<SdlSurface> mFontSdlSurface;
    float mElapsedTicks;
    float mLastFrameSeconds;
    Updater* mUpdater;
    bool mQuit;
        
    float mMouseX;
    float mMouseY;
    bool mMouseButtonDown;
	int mCurrentKeyCode;

    EngineImplementation()
    : mSdlWrapper(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)
    , mSdlWindow(WindowWidth, WindowHeight)
    , mGlContext(mSdlWindow)
    , mLastFrameSeconds(1.0f / 60.0f)
    , mMouseX(WindowWidth * 0.5f)
    , mMouseY(WindowHeight * 0.5f)
    , mMouseButtonDown(false)
	, mCurrentKeyCode(0)
    , mQuit(false)
    , mUpdater(nullptr)
    , mElapsedTicks(static_cast<float>(SDL_GetTicks()))
    {}

    void Start();
    void ParseEvents();
};

Engine::Engine(const char* assetsDirectory)
: mPimpl(new EngineImplementation) {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string assets(assetsDirectory);
	std::string background = assets + "/playfield.png";
	std::string avatarLeftOpen = assets + "/open_left_32.png";
	std::string avatarLeftClosed = assets + "/closed_left_32.png";
	std::string avatarRightOpen = assets + "/open_right_32.png";
	std::string avatarRightClosed = assets + "/closed_right_32.png";
	std::string avatarDownOpen = assets + "/open_down_32.png";
	std::string avatarDownClosed = assets + "/closed_down_32.png";
	std::string avatarUpOpen = assets + "/open_up_32.png";
	std::string avatarUpClosed = assets + "/closed_up_32.png";
	std::string ghostBlinky = assets + "/ghost_32_red.png";
	std::string ghostPinky = assets + "/ghost_32_pink.png";
	std::string ghostInky = assets + "/ghost_32_cyan.png";
	std::string ghostClyde = assets + "/ghost_32_orange.png";
	std::string ghostVulnerable = assets + "/ghost_vulnerable_32.png";
	std::string ghostDead = assets + "/ghost_dead_32.png";
	std::string dot = assets + "/small_dot_32.png";
	std::string bigDot = assets + "/big_dot_32.png";
	std::string font = assets + "/berlin_sans_demi_72_0.png";

    mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BACKGROUND].reset(new SdlSurface(background.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_LEFT_OPEN].reset(new SdlSurface(avatarLeftOpen.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_LEFT_CLOSED].reset(new SdlSurface(avatarLeftClosed.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_RIGHT_OPEN].reset(new SdlSurface(avatarRightOpen.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_RIGHT_CLOSED].reset(new SdlSurface(avatarRightClosed.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_DOWN_OPEN].reset(new SdlSurface(avatarDownOpen.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_DOWN_CLOSED].reset(new SdlSurface(avatarDownClosed.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_UP_OPEN].reset(new SdlSurface(avatarUpOpen.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_AVATAR_UP_CLOSED].reset(new SdlSurface(avatarUpClosed.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_BLINKY].reset(new SdlSurface(ghostBlinky.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_PINKY].reset(new SdlSurface(ghostPinky.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_INKY].reset(new SdlSurface(ghostInky.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_CLYDE].reset(new SdlSurface(ghostClyde.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_VULNERABLE].reset(new SdlSurface(ghostVulnerable.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GHOST_DEAD].reset(new SdlSurface(ghostDead.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_DOT].reset(new SdlSurface(dot.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BIG_DOT].reset(new SdlSurface(bigDot.c_str()));
	mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_CHERRY].reset(new SdlSurface(dot.c_str()));
	mPimpl->mFontSdlSurface.reset(new SdlSurface(font.c_str()));

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WindowWidth, WindowHeight, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

Engine::~Engine() {
}

float Engine::GetLastFrameSeconds() const {
    return mPimpl->mLastFrameSeconds;
}
    
float Engine::GetMouseX() const {
    return mPimpl->mMouseX;
}
    
float Engine::GetMouseY() const {
    return mPimpl->mMouseY;
}
    
bool Engine::GetMouseButtonDown() const {
    return mPimpl->mMouseButtonDown;
}

int Engine::GetCurrentKeyCode() const {
	return mPimpl->mCurrentKeyCode;
}

void Engine::Quit() {
    mPimpl->mQuit = true;
}
    
void Engine::Start(Updater& updater) {
    mPimpl->mUpdater = &updater;
    mPimpl->mSdlWindow.Show();
    mPimpl->Start();
}
    
int Engine::GetTextureHeight(Texture texture) const {
    return mPimpl->mSdlSurfaceContainer[texture]->Height();
}
    
int Engine::GetTextureWidth(Texture texture) const {
    return mPimpl->mSdlSurfaceContainer[texture]->Width();
}
    
void Engine::Render(Engine::Texture texture, const glm::mat4& transform) {
    glLoadMatrixf(reinterpret_cast<const float*>(&transform));
        
    SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];
        
    surface.Bind();
        
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex2i(0, surface.Height());
    glTexCoord2i(1, 1); glVertex2i(surface.Width(), surface.Height());
    glTexCoord2i(1, 0); glVertex2i(surface.Width(), 0);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glEnd();
}
    
void Engine::Render(Texture texture, float x, float y, float rotation) {
    glm::mat4 transformation;
    transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
    if (rotation) {
        transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    Render(texture, transformation);
}

Glyph& FindGlyph(char c) {
    auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
    if (found == std::end(Font) || c < *found) {
        found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
    }
    return *found;
}
    
float Engine::CalculateStringWidth(const char* text) const {
    int advance = 0;
    for (; *text; ++text) {
        Glyph& g = FindGlyph(*text);
        advance += g.advance;
    }
    return advance*TextScale;
}

void Engine::Write(const char* text, const glm::mat4& transform) {
    glLoadMatrixf(reinterpret_cast<const float*>(&transform));
    int advance = 0;
    for (; *text; ++text) {
        Glyph& g = FindGlyph(*text);
            
        float fontTexWidth  = static_cast<float>(mPimpl->mFontSdlSurface->Width());
        float fontTexHeight = static_cast<float>(mPimpl->mFontSdlSurface->Height());
            
        float uvLeft = static_cast<float>(g.x) / fontTexWidth;
        float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
        float uvBottom = static_cast<float>(g.y) / fontTexHeight;
        float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;
            
        float worldLeft = static_cast<float>(g.xoffset + advance);
        float worldRight = static_cast<float>(g.xoffset + g.width + advance);
        float worldBottom = static_cast<float>(g.yoffset);
        float worldTop = static_cast<float>(g.yoffset + g.height);
            
        mPimpl->mFontSdlSurface->Bind();
            
        glBegin(GL_QUADS);
        glTexCoord2f(uvLeft / 1.0f, uvTop / 1.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
        glTexCoord2f(uvRight / 1.0f, uvTop / 1.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
        glTexCoord2f(uvRight / 1.0f, uvBottom / 1.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
        glTexCoord2f(uvLeft / 1.0f, uvBottom / 1.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
        glEnd();
        advance += g.advance + 5;
    }
}
    
void Engine::Write(const char* text, float x, float y, float rotation) {
    glm::mat4 transformation;
    transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
    if (rotation) {
        transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        transformation = glm::translate(transformation, glm::vec3(-CalculateStringWidth(text)/2.0f, -20.0f, 0.0f));
    }
    Write(text, transformation);
}

int Engine::GetWidth() const {
    return WindowWidth;
}
    
int Engine::GetHeight() const {
    return WindowHeight;
}
    
void Engine::EngineImplementation::Start() {
    while (!mQuit) {
        SDL_GL_SwapWindow(mSdlWindow);
        glClear(GL_COLOR_BUFFER_BIT);
            
        ParseEvents();
            
        float currentTicks = static_cast<float>(SDL_GetTicks());
        float lastFrameTicks = currentTicks - mElapsedTicks;
        mElapsedTicks = currentTicks;
            
        lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
        mLastFrameSeconds = lastFrameTicks * 0.001f;
            
        if (mUpdater) {
            mUpdater->Update();
        }
    }
}
    
void Engine::EngineImplementation::ParseEvents() {
    SDL_Event event;

	while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
				mQuit = true;
			case SDL_KEYDOWN:
                break;
            case SDL_MOUSEBUTTONDOWN:
                mMouseButtonDown = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mMouseButtonDown = false;
                break;
            case SDL_MOUSEMOTION:
                mMouseX = static_cast<float>(event.motion.x);
                mMouseY = static_cast<float>(event.motion.y);
                break;
            default:
                break;
        }

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			mCurrentKeyCode = 1;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			mCurrentKeyCode = 2;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			mCurrentKeyCode = 3;
		}
		else if (currentKeyStates[SDL_SCANCODE_UP])
		{
			mCurrentKeyCode = 4;
		}
		else if (currentKeyStates[SDL_SCANCODE_SPACE])
		{
			mCurrentKeyCode = 5;
		}
		else if (currentKeyStates[SDL_SCANCODE_RETURN])
		{
			mCurrentKeyCode = 6;
		}
		else
		{
			mCurrentKeyCode = -1;
		}
	}
}
