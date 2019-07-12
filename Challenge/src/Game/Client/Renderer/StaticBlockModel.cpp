#include "StaticBlockModel.h"

StaticBlockModel::StaticBlockModel()
    :BlockModel(BlockModel::Type::Static) 
{

}

const std::vector<BlockModel::VertexFormat>& StaticBlockModel::getCullableFace(BlockModel::BlockFace side) const noexcept {
    return mModels[static_cast<int>(side)];
}

const std::vector<BlockModel::VertexFormat>& StaticBlockModel::getUncullableFace() const noexcept {
    return mModels[6];
}

StaticBlockModel& StaticBlockModel::setCullableFaceModel(BlockModel::BlockFace side, const std::vector<VertexFormat>& data) {
    mModels[static_cast<int>(side)] = data;
    return *this;
}

StaticBlockModel& StaticBlockModel::setUncullableFaceModel(BlockModel::BlockFace side, const std::vector<VertexFormat>& data) {
    mModels[7] = data;
    return *this;
}
