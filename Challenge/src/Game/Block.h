#pragma once
#include <string>
#include <cstdint>

class BlockManager;

class Block {
public:
    Block(const Block&) = default;
    Block(Block&&) = default;
    std::uint32_t registerBlock(std::uint16_t domain) const;
    Block& setName(const std::string& name);
    const std::string& getName() const noexcept;
private:
    friend class BlockManager;
    explicit Block(BlockManager& blockManager) noexcept;
    BlockManager& mBlockManager;
    std::string mName;
};
