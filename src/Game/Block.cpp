#include "Block.h"
#include "BlockManager.h"

Block::Block(BlockManager& blockManager) noexcept
    :mBlockManager(blockManager), mName("")
{

}

std::uint32_t Block::registerBlock(std::uint16_t domain) const {
    return mBlockManager.registerBlock(domain, *this);
}

Block& Block::setName(const std::string& name) {
    mName = name;
    return *this;
}

const std::string& Block::getName() const noexcept {
    return mName;
}
