#pragma once
#include "Util/Nonmovable.h"
#include "Util/Noncopyable.h"
#include <SDL.h>
#include <glm/glm.hpp>
#undef main

class Interface 
    :private Nonmovable, private Noncopyable
{
public:
    static Interface& getInterface();
    void swapWindow() noexcept;
    void handleWindowEvents() noexcept;
    bool shouldLeave() const noexcept;
    glm::vec2 getRotationDelta() const noexcept;

    static constexpr int
        OPENGL_VERSION_MAJOR = 3,
        OPENGL_VERSION_MINOR = 3;
private:
    Interface();
    SDL_Window* mWindow;
    SDL_GLContext mContext;
    bool mShouldLeave;
    mutable glm::vec2 mRotationDelta;
    glm::vec2 lastMousePos;
};

