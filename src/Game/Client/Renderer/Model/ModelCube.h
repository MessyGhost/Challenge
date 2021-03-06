#pragma once
#include "../StaticBlockModel.h"
#include <cstdint>

class ModelCube 
    :public StaticBlockModel
{
public:
    /*Input: Indices of texture array. */
    ModelCube(std::uint32_t px, std::uint32_t nx,
              std::uint32_t py, std::uint32_t ny,
              std::uint32_t pz, std::uint32_t nz) noexcept;
};
