#include "BlockModel.h"
#include <glm/glm.hpp>

const std::vector<BlockModel::VertexFormat>& BlockModel::getCullableFace(BlockModel::BlockFace side) const noexcept {
    return mModels[static_cast<int>(side)];
}

const std::vector<BlockModel::VertexFormat>& BlockModel::getUncullableFace() const noexcept {
    return mModels[6];
}

BlockModel& BlockModel::setCullableFaceModel(BlockModel::BlockFace side, const std::vector<VertexFormat>& data) noexcept {
    mModels[static_cast<int>(side)] = data;
    return *this;
}

BlockModel& BlockModel::setUncullableFaceModel(BlockModel::BlockFace side, const std::vector<VertexFormat>& data) noexcept {
    mModels[7] = data;
    return *this;
}

