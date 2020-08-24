#pragma once
#include <map>
#include <cstdint>
#include <memory>
#include "Renderer/BlockModel.h"

class BlockModelManager {
public:
    BlockModelManager() = default;
    BlockModelManager(const BlockModelManager&) = default;
    BlockModelManager(BlockModelManager&&) = default;
    const BlockModel& getModelByBlockId(std::uint32_t id) const;
    void registerModel(std::uint16_t domain, std::uint16_t block, std::unique_ptr<BlockModel>&& model);
private:
    mutable std::map<std::uint32_t, std::unique_ptr<BlockModel>> mModels;
};
