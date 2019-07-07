#pragma once
#include "../Util/Noncopyable.h"
#include "../Util/Nonmovable.h"

class Game 
    :private Noncopyable, private Nonmovable
{
public:
    Game() noexcept;
    void run();
//private:
    
};
