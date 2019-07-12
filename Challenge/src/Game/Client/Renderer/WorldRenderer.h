#pragma once
#include "../../World/ChunkCache.h"
#include "../BlockModelManager.h"
#include "../../../Util/Noncopyable.h"
#include "../../../Util/Nonmovable.h"
#include "../../../Graphic/VertexArray.h"
#include <glm/glm.hpp>
#include <list>
#include <map>
#include <chrono>
#include <cstdint>

class WorldRenderer 
    :private Noncopyable, private Nonmovable
{
public:
    WorldRenderer(const ChunkCache& chunkCache, const BlockModelManager& blockModelManager);
    void render(glm::dvec3 center, std::uint8_t radiusDistance);
private:
    const ChunkCache& mChunkCache;
    const BlockModelManager& mBlockModelManager;
    std::map<ChunkCache::ChunkPos, std::chrono::time_point<std::chrono::system_clock>>
        mChunkVerticesMakingTimes;
    struct ChunkRenderNode {
        gl::VertexArray vertexArray;
        gl::VertexBuffer modelBuffer;
        gl::VertexBuffer positionBuffer;
        std::uint32_t numVertices;
    };
    std::list<ChunkRenderNode> mNodes;
};
