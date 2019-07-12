#pragma once
#include "../Util/Noncopyable.h"
#include "../Util/Nonmovable.h"
#include "GameResource.h"

class Game 
    :private Noncopyable, private Nonmovable
{
public:
    Game(const GameResource& gamerc) noexcept;
    void run();
private:
    const GameResource& mGameResource;
};
