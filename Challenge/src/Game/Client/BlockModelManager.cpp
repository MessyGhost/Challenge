#include "BlockModelManager.h"
#include "../../Util/LoggerDefinition.h"
#include <stdexcept>
#include <sstream>

const BlockModel& BlockModelManager::getModelByBlockId(std::uint32_t id) const {
    return *mModels[id];
}

void BlockModelManager::registerModel(std::uint16_t domain, std::uint16_t block, std::unique_ptr<BlockModel>&& model) {
    if(mModels.find(domain << 16 | block) != mModels.end()) {
        std::stringstream buffer;
        buffer << "Tried registering an registered model.\n"
               << "BlockId: " << (domain << 16 | block) << " (domain: " << domain << ", id: " << block << ").";
        throw std::runtime_error(buffer.str());
    }
    mModels.emplace(std::piecewise_construct, std::make_tuple(domain << 16 | block), std::forward_as_tuple(std::move(model)));
    debugstream << "Block model for block " << (domain << 16 | block) << "(domain: " << domain 
                << ", id: " << block <<") was registered.";
}
