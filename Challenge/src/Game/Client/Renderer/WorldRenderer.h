#pragma once
#include "../../World/ChunkCache.h"
#include "../BlockModelManager.h"
#include "../../../Util/Noncopyable.h"
#include "../../../Util/Nonmovable.h"
#include "../../../Graphic/VertexArray.h"
#include "../../../Graphic/ShaderProgram.h"
#include <glm/glm.hpp>
#include <map>
#include <list>
#include <chrono>
#include <cstdint>

class WorldRenderer 
    :private Noncopyable, private Nonmovable
{
public:
    WorldRenderer(const ChunkCache& chunkCache, const BlockModelManager& blockModelManager, gl::ShaderProgram& shader);
    void render(glm::dvec3 center, std::uint8_t radiusDistance);
private:
    const ChunkCache& mChunkCache;
    const BlockModelManager& mBlockModelManager;
    gl::ShaderProgram& mBlockRenderingShader;
    struct ChunkMesh {
        ChunkMesh(ChunkMesh&&) = default;
        gl::VertexArray vertexArray;
        gl::VertexBuffer modelBuffer;
        gl::VertexBuffer positionBuffer;
        std::uint32_t numVertices;
        std::chrono::time_point<std::chrono::system_clock> lastChange;
    };
    std::map<ChunkCache::ChunkPos, ChunkMesh> mMeshes;
    std::list<ChunkMesh> mUnusedChunkMeshes;
    ChunkMesh genUnusedChunkMesh(
        std::uint32_t modelBufferSize, const void* modelBufferData,
        std::uint32_t positionBufferSize, const void* positionBufferData, std::uint32_t numVertices) noexcept;
    void recyleChunkMesh(ChunkMesh&& mesh) noexcept;
};
