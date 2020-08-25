#include "BlockManager.h"
#include "../Util/LoggerDefinition.h"
#include <stdexcept>
#include <climits>
#include <sstream>

BlockManager::BlockManager() 
{

}

BlockManager::blockstorageid_t BlockManager::registerBlock(std::uint16_t domain, const Block& block) {
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
    blockstorageid_t blockId;
    pBlocks->emplace_back(block);
    blockId = (blockstorageid_t)pBlocks->size();
    if(blockId > BLOCKID_TYPE_MAX) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Too many blocks are registered in a same domain: " << domain << ".";
        throw std::overflow_error(buffer.str());
    }
    debugstream << "New block (domain: " << domain <<", id: " << blockId << ", name: " << block.getName() << ") was registered.";
    return domain << 16 | blockId;
}

BlockManager::blockstorageid_t BlockManager::registerBlock(BlockManager::domainid_t domain, Block&& block) {
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
    blockstorageid_t blockId;
    pBlocks->emplace_back(block);
    blockId = (blockstorageid_t)pBlocks->size();
    if(blockId > BLOCKID_TYPE_MAX) {
        pBlocks->pop_back();
        std::stringstream buffer;
        buffer << "Too many blocks are registered in a same domain: " << domain << ".";
        throw std::overflow_error(buffer.str());
    }
    debugstream << "New block (domain: " << domain <<", id: " << blockId << ", name: " << block.getName() << ") was registered.";
    return domain << 16 | blockId;
}

const Block& BlockManager::getBlockById(BlockManager::blockstorageid_t id) const {
    return mBlocks.at(id >> 16).at(id & 0xffff);
}

BlockManager::domainid_t BlockManager::genUnusedDomain() {
    if(mBlocks.size() == DOMAINID_TYPE_MAX) {
        throw std::overflow_error("No more unused domain can be generated.");
    }
    mBlocks.emplace_back(std::vector<Block>());
    debugstream << "New block domain(id: " << mBlocks.size() - 1 << ") was generated.";
    return (domainid_t)(mBlocks.size() - 1);
}

Block BlockManager::genBlock() noexcept {
    return Block(*this);
}
