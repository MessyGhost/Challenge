#pragma once
#include <vector>
#include <cstdint>

class BlockModel {
public:
    struct VertexFormat {
        float x, y, z;
        float s, t, r;
        enum : std::uint32_t {
            SizeOfPosition = 3 * sizeof(float),
            SizeOfTextureCoordinate = 3 * sizeof(float),
            Size = SizeOfPosition + SizeOfTextureCoordinate
        };
    };
    enum class Type : std::uint8_t {
        Dynamic,
        Static
    };
    enum BlockFace : std::uint8_t {
        PositiveX = 0,
        PositiveY,
        PositiveZ,
        NegativeX,
        NegativeY,
        NegativeZ,
        FaceCount
    };
    BlockModel(const BlockModel&) = default;
    BlockModel(BlockModel&&) = default;
    virtual ~BlockModel() = default;
    virtual const std::vector<VertexFormat>& getCullableFace(BlockFace side) const noexcept = 0;
    virtual const std::vector<VertexFormat>& getUncullableFace() const noexcept = 0;
    Type getType() const noexcept;
protected:
    BlockModel(Type type);
private:
    Type mType;
};
