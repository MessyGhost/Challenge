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
    using blockstorageid_t = std::uint32_t;
    using domainid_t = std::uint16_t;
    using blockid_t = std::uint16_t;
    static constexpr domainid_t DOMAINID_TYPE_MAX = UINT16_MAX;
    static constexpr blockid_t BLOCKID_TYPE_MAX = UINT16_MAX;
    BlockManager();
    blockstorageid_t registerBlock(std::uint16_t domain, const Block& block);
    blockstorageid_t registerBlock(std::uint16_t domain, Block&& block);
    const Block& getBlockById(blockstorageid_t id) const;
    domainid_t genUnusedDomain();
    Block genBlock() noexcept;
private:
    std::vector<std::vector<Block>> mBlocks;
};
