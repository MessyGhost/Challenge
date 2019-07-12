#include "ChunkCache.h"
#include "../../Util/LoggerDefinition.h"

bool ChunkCache::ChunkPos::operator<(const ChunkCache::ChunkPos& lhs) const noexcept {
    if(x < lhs.x) {
        return true;
    }
    if(y < lhs.y) {
        return true;
    }
    if(z < lhs.z) {
        return true;
    }
    return false;
}

void ChunkCache::put(const ChunkCache::ChunkPos& chunkPos, Chunk&& chunk) noexcept {
    if(mChunks.erase(chunkPos) > 0) {
        debugstream << "The chunk at ( " << chunkPos.x <<", "<< chunkPos.y <<", "<< chunkPos.z << "has been merged.";
    }
    mChunks.insert(std::make_pair(chunkPos, std::move(chunk)));
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
