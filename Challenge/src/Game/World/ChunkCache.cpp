#include "ChunkCache.h"
#include "../../Util/LoggerDefinition.h"
#include <new>

bool ChunkCache::ChunkPos::operator<(const ChunkCache::ChunkPos& other) const noexcept {
    if(x < other.x) {
        return true;
    }
    else if(x == other.x){
        if(y < other.y) {
            return true;
        }
        else if(y == other.y && z < other.z) {
            return true;
        }
    }
    return false;
}

void ChunkCache::put(const ChunkCache::ChunkPos& chunkPos, Chunk&& chunk) noexcept {
    decltype(mChunks)::iterator p;
    if((p = mChunks.find(chunkPos)) != mChunks.end()) {
        p->second.~Chunk();
        new (&p->second) Chunk(std::move(chunk));
        debugstream << "The chunk at ( " << chunkPos.x <<", "<< chunkPos.y <<", "<< chunkPos.z << "has been merged.";
    }
    mChunks.emplace(std::piecewise_construct ,std::make_tuple(chunkPos), std::make_tuple(std::move(chunk)));
}

const Chunk& ChunkCache::getChunk(const ChunkCache::ChunkPos& chunkPos) const {
    return mChunks.at(chunkPos);
}

Chunk& ChunkCache::getChunk(const ChunkCache::ChunkPos& chunkPos) {
    return mChunks.at(chunkPos);
}

const std::map<ChunkCache::ChunkPos, Chunk>& ChunkCache::getChunksReference() const noexcept {
    return mChunks;
}

bool ChunkCache::contains(const ChunkCache::ChunkPos& pos) const noexcept {
    return mChunks.find(pos) != mChunks.end();
}

ChunkCache::ChunkPos::ChunkPos() noexcept
    :x(0), y(0), z(0)
{

}

ChunkCache::ChunkPos::ChunkPos(std::int32_t x, std::int32_t y, std::int32_t z) noexcept
    :x(x), y(y), z(z)
{

}
