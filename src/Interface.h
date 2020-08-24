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
    glm::dvec2 getRotationDelta() const noexcept;
    struct MoveIntent {
        double distanceOnPlane;
        double yaw;
        double deltaOnVertical;
    };
    void getMoveIntent(MoveIntent& moveIntent) const noexcept;
    void setMouseVisible(bool visible) noexcept;
    void present() noexcept;
    static constexpr int
        OPENGL_VERSION_MAJOR = 3,
        OPENGL_VERSION_MINOR = 3;
private:
    Interface();
    SDL_Window* mWindow;
    SDL_GLContext mContext;
    bool mShouldLeave;
    bool mHasFocus;
    mutable glm::dvec2 mRotationDelta;
};

