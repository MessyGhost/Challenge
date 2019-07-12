#include "Interface.h"
#include "Util/LoggerDefinition.h"
#include <stdexcept>
#include <GL/glew.h>
#include <SDL_net.h>
#include <glm/gtc/constants.hpp>

Interface& Interface::getInterface() {
    static Interface instance;
    return instance;
}

void Interface::swapWindow() noexcept {
    SDL_GL_SwapWindow(mWindow);
}

void Interface::handleWindowEvents() noexcept {
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        switch(event.type) {
        case SDL_WINDOWEVENT:
        {
            switch(event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
            {
                mShouldLeave = true;
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                mHasFocus = true;
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                mHasFocus = false;
                break;
            }
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            if(SDL_GetMouseFocus() == mWindow) {
                int w, h;
                SDL_GetWindowSize(mWindow, &w, &h);
                mRotationDelta += glm::vec2(-event.motion.yrel, event.motion.xrel);
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                mShouldLeave = true;
            }
            break;
        }
        case SDL_QUIT:
        {
            mShouldLeave = true;
            break;
        }
        }
    }
}

bool Interface::shouldLeave() const noexcept {
    return mShouldLeave;
}

Interface::Interface() 
    :mShouldLeave(false), mRotationDelta(0.0f, 0.0f), mHasFocus(false)
{
    debugstream << "An instance of Interface was created.";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("Cannot init SDL.");
    }
    else if (SDLNet_Init() != 0) {
        throw std::runtime_error("Cannot init SDLNet");
    }

    mWindow = SDL_CreateWindow("Challenge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                    800, 600, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);

    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, mContext);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Cannot init OpenGL(GLEW).");
    }

    infostream << "OpenGL initialized successfully.\n" 
               << "GL_VERSION: " << glGetString(GL_VERSION) << '.';
    infostream << "But working on OpenGL " << OPENGL_VERSION_MAJOR <<'.'<< OPENGL_VERSION_MINOR;
}

glm::vec2 Interface::getRotationDelta() const noexcept {
    glm::vec2 result = mRotationDelta;
    mRotationDelta.x = mRotationDelta.y = 0.0f;
    return result;
}

void Interface::getMoveIntent(MoveIntent& intent) const noexcept {
    if(SDL_GetKeyboardFocus() == mWindow) {
        
        bool firstCase = true;
        auto rotate = [&](float radians) {
            if(firstCase) {
                firstCase = false;
                intent.yaw = radians;
            }
            else
            {
                intent.yaw = glm::mix(intent.yaw, radians, 0.5);
            }
        };
        intent.distanceOnPlane = intent.deltaOnVertical = 0.0f;
        const std::uint8_t* keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S]) {
            rotate(0.0f);
            intent.distanceOnPlane = 1.0f;
        }
        if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
            rotate(glm::pi<float>() * 1.5f);
            intent.distanceOnPlane = 1.0f;
        }
        if(keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_A]) {
            rotate(glm::pi<float>());
            intent.distanceOnPlane = 1.0f;
        }
        if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
            rotate(glm::pi<float>() * 0.5f);
            intent.distanceOnPlane = 1.0f;
        }
        if(keys[SDL_SCANCODE_SPACE]) {
            intent.deltaOnVertical += 1.0f;
        }
        if(keys[SDL_SCANCODE_LSHIFT]) {
            intent.deltaOnVertical -= 1.0f;
        }
        return;
    }
    else
    {
        intent.distanceOnPlane = 0.0f;
        intent.deltaOnVertical = 0.0f;
        return;
    }
}

void Interface::setMouseVisible(bool visible) noexcept {
    SDL_SetRelativeMouseMode(visible ? SDL_FALSE : SDL_TRUE);
}

void Interface::present() noexcept {
    SDL_ShowWindow(mWindow);
}
