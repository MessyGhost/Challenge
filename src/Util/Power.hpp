#pragma once

/*Only works on integers. */
template<unsigned int base, unsigned int exponent>
class Power {
public:
    Power() = delete;
    constexpr static unsigned int value = base * Power<base, (exponent - 1)>::value;
};

template<unsigned int base>
class Power<base, 0> {
public:
    Power() = delete;
    constexpr static unsigned int value = 1;
};

template<>
class Power<0, 0> {
public:
    Power() = delete;
    constexpr static unsigned int value = 0;
};
