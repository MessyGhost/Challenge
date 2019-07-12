#include "BlockManager.h"
#include "../Util/LoggerDefinition.h"
#include <stdexcept>
#include <climits>
#include <sstream>

BlockManager::BlockManager() 
{

}

std::uint32_t BlockManager::registerBlock(std::uint16_t domain, const Block& block) {
    std::vector<Block>* pBlocks = nullptr;
    try {
        pBlocks = &mBlocks.at(domain);
    }
    catch (std::out_of_range) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Domain not found: " << domain << ".";
        throw std::invalid_argument(buffer.str());
    }
    std::uint32_t blockId;
    pBlocks->push_back(block);
    blockId = pBlocks->size();
    if(blockId > INT16_MAX) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Too many blocks are registered in a same domain: " << domain << ".";
        throw std::overflow_error(buffer.str());
    }
    debugstream << "New block (domain: " << domain <<", id: " << blockId << ", name: " << block.getName() << ") was registered.";
    return domain << 16 | blockId;
}

std::uint32_t BlockManager::registerBlock(std::uint16_t domain, Block&& block) {
    std::vector<Block>* pBlocks = nullptr;
    try {
        pBlocks = &mBlocks.at(domain);
    }
    catch (std::out_of_range) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Domain not found: " << domain << ".";
        throw std::invalid_argument(buffer.str());
    }
    std::uint32_t blockId;
    pBlocks->push_back(block);
    blockId = pBlocks->size();
    if(blockId > INT16_MAX) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Too many blocks are registered in a same domain: " << domain << ".";
        throw std::overflow_error(buffer.str());
    }
    debugstream << "New block (domain: " << domain <<", id: " << blockId << ", name: " << block.getName() << ") was registered.";
    return domain << 16 | blockId;
}

const Block& BlockManager::getBlockById(std::uint32_t id) const {
    return mBlocks.at(id >> 16).at(id & 0xffff);
}

std::uint16_t BlockManager::genUnusedDomain() {
    if(mBlocks.size() == INT16_MAX) {
        throw std::overflow_error("No more unused domain can be generated.");
    }
    mBlocks.push_back(std::vector<Block>());
    debugstream << "New block domain(id: " << mBlocks.size() - 1 << ") was generated.";
    return mBlocks.size() - 1;
}

Block BlockManager::genBlock() noexcept {
    return Block(*this);
}
