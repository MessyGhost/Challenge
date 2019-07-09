#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

class BlockModel {
public:
    enum BlockFace : unsigned char {
        PositiveX = 0,
        PositiveY,
        PositiveZ,
        NegativeX,
        NegativeY,
        NegativeZ,
        FaceCount
    };
    struct VertexFormat {
        float x, y, z;
        float s, t, r;
    };
    BlockModel() = default;
    BlockModel(BlockModel&) = default;
    BlockModel(BlockModel&&) = default;
    const std::vector<VertexFormat>& getCullableFace(BlockFace side) const noexcept;
    const std::vector<VertexFormat>& getUncullableFace() const noexcept;
    BlockModel& setCullableFaceModel(BlockFace side, const std::vector<VertexFormat>& data) noexcept;
    BlockModel& setUncullableFaceModel(BlockFace side, const std::vector<VertexFormat>& data) noexcept;
private:
    std::array<std::vector<VertexFormat>, 7> mModels;
};
