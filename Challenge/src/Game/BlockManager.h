#pragma once
#include "Block.h"
#include "../Util/Noncopyable.h"
#include <vector>
#include <map>
#include <cstdint>

class BlockManager
    :private Noncopyable
{
public:
    BlockManager();
    std::uint32_t registerBlock(std::uint16_t domain, const Block& block);
    std::uint32_t registerBlock(std::uint16_t domain, Block&& block);
    const Block& getBlockById(std::uint32_t id) const;
    std::uint16_t genUnusedDomain();
    Block genBlock() noexcept;
private:
    std::vector<std::vector<Block>> mBlocks;
};
