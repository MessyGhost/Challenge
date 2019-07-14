#pragma once
#include "ChunkCache.h"
#include <cstdint>

class World {
public:
    World() = default;
    ~World() = default;
    void setblock(std::int64_t x, std::int64_t y, std::int64_t z, std::uint32_t block);
    void loadChunk(ChunkCache::ChunkPos where);
    const ChunkCache& getChunkCache() const noexcept;
private:
    static void fillChunk(Chunk& chunk, const Chunk::BlockPos& from, const Chunk::BlockPos& to, std::uint32_t block);
    ChunkCache mChunkCache;
};
