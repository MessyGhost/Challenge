#pragma once
#include "../../Util/Noncopyable.h"
#include "../../Util/Nonmovable.h"
#include "Chunk.h"
#include <map>
#include <cstdint>

class ChunkCache 
    :private Noncopyable, private Nonmovable
{
public:
    struct ChunkPos {
        ChunkPos() noexcept;
        ChunkPos(std::int32_t x, std::int32_t y, std::int32_t z) noexcept;
        bool operator<(const ChunkPos& other) const noexcept;
        std::int32_t x, y, z;
    };
    ChunkCache() = default;
    void put(const ChunkPos& chunkPos, Chunk&& chunk) noexcept;
    const Chunk& getChunk(const ChunkPos& chunkPos) const;
    Chunk& getChunk(const ChunkPos& chunkPos);
    const std::map<ChunkPos, Chunk>& getChunksReference() const noexcept;
    bool contains(const ChunkPos& pos) const noexcept;
private:
    std::map<ChunkPos, Chunk> mChunks;
};
