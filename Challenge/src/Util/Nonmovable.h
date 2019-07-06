#pragma once

class Nonmovable {
public:
    Nonmovable() = default;
    Nonmovable(Nonmovable&&) = delete;
    Nonmovable& operator=(Nonmovable&&) = delete;
    Nonmovable(const Nonmovable&) = default;
    Nonmovable& operator=(const Nonmovable&) = default;
};
