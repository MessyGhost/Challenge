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
    infostream << "But working on OpenGL " << OPENGL_VERSION_MAJOR <<'.'<< OPENGL_VERSION_MINOR << '.';
}

glm::dvec2 Interface::getRotationDelta() const noexcept {
    glm::dvec2 result = mRotationDelta;
    mRotationDelta.x = mRotationDelta.y = 0.0f;
    return result;
}

void Interface::getMoveIntent(MoveIntent& intent) const noexcept {
    if(SDL_GetKeyboardFocus() == mWindow) {
        intent.distanceOnPlane = intent.deltaOnVertical = 0.0;
        const std::uint8_t* keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S]) {
            intent.distanceOnPlane = 1.0;
            if(keys[SDL_SCANCODE_A]) {
                intent.yaw = glm::pi<double>() * 1.75;
            }
            else if(keys[SDL_SCANCODE_D]) {
                intent.yaw = glm::pi<double>() * 0.25;
            }
            else {
                intent.yaw = 0.0;
            }
        }
        else if (keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_W]) {
            intent.distanceOnPlane = 1.0;
            if(keys[SDL_SCANCODE_A]) {
                intent.yaw = glm::pi<double>() * 1.25;
            }
            else if(keys[SDL_SCANCODE_D]) {
                intent.yaw = glm::pi<double>() * 0.75;
            }
            else {
                intent.yaw = glm::pi<double>();
            }
        }
        else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
            intent.distanceOnPlane = 1.0;
            intent.yaw = glm::pi<double>() * 1.5;
        }
        else if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
            intent.distanceOnPlane = 1.0;
            intent.yaw = glm::half_pi<double>();
        }

        if(keys[SDL_SCANCODE_SPACE] && !(keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT])) {
            intent.deltaOnVertical = 1.0;
        }
        else if((keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]) && !keys[SDL_SCANCODE_SPACE]) {
            intent.deltaOnVertical = -1.0;
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
