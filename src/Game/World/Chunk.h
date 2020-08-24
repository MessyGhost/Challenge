#pragma once
#include "../../Util/Noncopyable.h"
#include "../../Util/Nonmovable.h"
#include "../../Util/Power.hpp"
#include <array>
#include <cstdint>
#include <chrono>
#include <memory>

class Chunk 
    :private Noncopyable
{
public:
    static constexpr std::uint8_t CHUNK_SIZE = 32;
    enum class State {
        Empty,
        Full,
        Generating
    };
    struct BlockPos {
        std::uint8_t x, y, z;
    };
    using BlockData = std::array<std::uint32_t, Power<CHUNK_SIZE, 3>::value>;
    Chunk() noexcept;
    Chunk(const BlockData& blocks);
    Chunk(BlockData&& blocks);
    Chunk(Chunk&&) noexcept;
    ~Chunk() = default;
    Chunk& operator=(Chunk&& rhs) = delete;
    std::uint32_t getBlock(const BlockPos& blockPos) const;
    void setBlock(const BlockPos& blockPos, std::uint32_t block);
    State getState() const noexcept;
    void setState(State state) noexcept;
    const std::chrono::time_point<std::chrono::system_clock>& getLastWriting() const noexcept;
private:
    std::unique_ptr<BlockData> m_pBlocks;
    State mState;
    mutable std::chrono::time_point<std::chrono::system_clock> mLastWriting;
};
