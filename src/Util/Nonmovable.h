#pragma once

class Nonmovable {
protected:
    Nonmovable() = default;
    ~Nonmovable() = default;
    Nonmovable(Nonmovable&&) = delete;
    Nonmovable& operator=(Nonmovable&&) = delete;
};
