#include "World.h"
#include <cmath>

void World::setblock(std::int64_t x, std::int64_t y, std::int64_t z, std::uint32_t block) {
    //Chunk pos
    std::int32_t 
        cx = std::floor(x / (double)Chunk::CHUNK_SIZE),
        cy = std::floor(y / (double)Chunk::CHUNK_SIZE),
        cz = std::floor(z / (double)Chunk::CHUNK_SIZE);
    //Pos in chunk
    std::uint8_t 
        xInC = x % Chunk::CHUNK_SIZE,
        yInC = y % Chunk::CHUNK_SIZE,
        zInC = z % Chunk::CHUNK_SIZE;
    mChunkCache.getChunk(ChunkCache::ChunkPos{cx, cy, cz}).getBlock(Chunk::BlockPos{xInC, yInC, zInC});
}

void World::loadChunk(ChunkCache::ChunkPos where) {
    if(mChunkCache.contains(where))return;
    Chunk::BlockData data;
    data.fill(0);
    Chunk c(std::move(data));
    fillChunk(c, Chunk::BlockPos{0, 0, 0}, Chunk::BlockPos{Chunk::CHUNK_SIZE - 1, 10, Chunk::CHUNK_SIZE - 1}, 1);
    mChunkCache.put(where, std::move(c));
}

const ChunkCache& World::getChunkCache() const noexcept {
    return mChunkCache;
}

void World::fillChunk(Chunk& chunk, const Chunk::BlockPos& from, const Chunk::BlockPos& to, std::uint32_t block) {
    std::uint8_t x, y, z;
    for(x = from.x; x <= to.x; ++x)for(y = from.y; y <= to.y; ++y)for(z = from.z; z <= to.z; ++z) {
        chunk.setBlock(Chunk::BlockPos{x, y, z}, block);
    }
}
