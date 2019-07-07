#include "Interface.h"
#include <stdexcept>
#include <GL/glew.h>
#include <SDL_net.h>

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
        if (event.type == SDL_WINDOWEVENT) {
            switch(event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
            {
                mShouldLeave = true;
                break;
            }
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            static glm::vec2 lastPos = glm::vec2(event.motion.x, event.motion.y);
            mRotationDelta += glm::vec2((lastPos.y - event.motion.y), (event.motion.x - lastPos.x));
            lastPos = glm::vec2(event.motion.x, event.motion.y);
        }
    }
}

bool Interface::shouldLeave() const noexcept {
    return mShouldLeave;
}

Interface::Interface() 
    :mShouldLeave(false), mRotationDelta(0.0f, 0.0f)
{
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
                    800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, mContext);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Cannot init OpenGL(GLEW).");
    }
}

glm::vec2 Interface::getRotationDelta() const noexcept {
    glm::vec2 result = mRotationDelta;
    mRotationDelta.x = mRotationDelta.y = 0.0f;
    return result;
}
