#pragma once
#include "BlockModel.h"
#include <array>

class StaticBlockModel
    :public BlockModel
{
public:
    StaticBlockModel();
    StaticBlockModel(const StaticBlockModel&) = default;
    StaticBlockModel(StaticBlockModel&&) = default;
    virtual ~StaticBlockModel() = default;
    StaticBlockModel& setCullableFaceModel(BlockFace side, const std::vector<VertexFormat>& data);
    StaticBlockModel& setUncullableFaceModel(BlockFace side, const std::vector<VertexFormat>& data);
    virtual const std::vector<VertexFormat>& getCullableFace(BlockFace side) const noexcept override final;
    virtual const std::vector<VertexFormat>& getUncullableFace() const noexcept override final;
private:
    std::array<std::vector<VertexFormat>, 7> mModels;
};
