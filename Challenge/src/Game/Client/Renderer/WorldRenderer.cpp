#include "WorldRenderer.h"
#include <thread>
#include <functional>
#include <future>
#include <mutex>

WorldRenderer::WorldRenderer(const ChunkCache& chunkCache, const BlockModelManager& blockModelManager)
    :mChunkCache(chunkCache), mBlockModelManager(blockModelManager)
{

}

void WorldRenderer::render(glm::dvec3 center, std::uint8_t radiusDistance) {
    auto& chunkCache = mChunkCache;
    auto& chunkVerticesMakingTimes = mChunkVerticesMakingTimes;
    auto& blockModelManager = mBlockModelManager;
    std::mutex write;

    struct TypeVertices {
        std::vector<BlockModel::VertexFormat> models;
        struct Position {
            float x, y, z;
        };
        std::vector<Position> positions;
    };

    auto makeChunkMesh = [&](const ChunkCache::ChunkPos& chunkPos) {
        TypeVertices result;
        try {
            if(std::chrono::system_clock::now() > chunkVerticesMakingTimes.at(chunkPos)) {
                return result;
            }
        }
        catch(std::out_of_range&) {
            chunkVerticesMakingTimes.insert(std::make_pair(chunkPos, std::chrono::system_clock::now()));
        }

        int chunkSize = Chunk::CHUNK_SIZE;
        const Chunk* pChunk;
        try{pChunk = &chunkCache.getChunk(chunkPos);}catch(std::out_of_range&) {
            return result;
        }
        auto& chunk = *pChunk;
        std::uint8_t x, y, z;
        auto insertModel = [&](const std::vector<BlockModel::VertexFormat>& model) {
            if(!model.empty()) {
                result.models.insert(result.models.end(), model.begin(), model.end());
                for(auto j = model.size(); j > 0; --j) {
                    result.positions.push_back({
                        (float)((double)chunkPos.x * (double)Chunk::CHUNK_SIZE + x - center.x),
                        (float)((double)chunkPos.y * (double)Chunk::CHUNK_SIZE + y - center.y),
                        (float)((double)chunkPos.z * (double)Chunk::CHUNK_SIZE + z - center.z)
                    });
                }
            }
        };
        for(x = 0; x < chunkSize; ++x)for(y = 0; y < chunkSize; ++y)for(z = 0; z < chunkSize; ++z) {
            auto block = chunk.getBlock(Chunk::BlockPos{x, y, z});
            if(!block)continue;
            auto& blockModel = blockModelManager.getModelByBlockId(block);
            for(std::uint8_t i = 0; i < BlockModel::FaceCount; ++i) {
                auto& model = blockModel.getCullableFace((BlockModel::BlockFace)i);
                insertModel(model);
            }
            auto& model = blockModel.getUncullableFace();
            insertModel(model);
        }

        /*Write when the chunk vertices update so that the vertices won't be update during next call. */
        std::lock_guard<std::mutex> writingGuard(write);
        try {
            auto& time = chunkVerticesMakingTimes.at(chunkPos);
            time += (std::chrono::system_clock::now() - time);
        }
        catch(std::out_of_range&) {
            chunkVerticesMakingTimes.insert(std::make_pair(chunkPos, std::chrono::system_clock::now()));
            return result;
        }
        return result;
    };
    std::list<std::future<TypeVertices>> verticesList;
    ChunkCache::ChunkPos pos{1, 0, 0};
    for(std::int32_t x = center.x - radiusDistance; x < center.x + radiusDistance; ++x)
     for(std::int32_t y = center.y - radiusDistance; y < center.y + radiusDistance; ++y)
      for(std::int32_t z = center.z - radiusDistance; z < center.z + radiusDistance; ++z) {
          verticesList.push_back(std::async(makeChunkMesh, ChunkCache::ChunkPos{x, y, z}));
      }

    for (auto& pVertices : verticesList) {
        const auto& vertices = pVertices.get();
        mNodes.push_back({
            gl::VertexArray(), 
            gl::VertexBuffer(
                sizeof(BlockModel::VertexFormat) * vertices.models.size(),
                gl::BufferUsage::DynamicDraw,
                vertices.models.data()
            ),
            gl::VertexBuffer(
                sizeof(float) * 3 * vertices.positions.size(),
                gl::BufferUsage::DynamicDraw,
                vertices.positions.data()
            ),
            0
            }
            );
        auto& node = mNodes.back();

        node.vertexArray.setAttribute(node.modelBuffer, 0, 3, gl::DataType::Float, false, BlockModel::VertexFormat::Size);
        node.vertexArray.setAttribute(node.modelBuffer, 1, 3, gl::DataType::Float, false, 
            BlockModel::VertexFormat::Size, BlockModel::VertexFormat::SizeOfPosition);
        node.vertexArray.setAttribute(node.positionBuffer, 2, 3, gl::DataType::Float, false);
        node.numVertices = vertices.models.size();
        node.vertexArray.enableAttribute(0);
        node.vertexArray.enableAttribute(1);
        node.vertexArray.enableAttribute(2);
    }

    for (auto& node :mNodes) {
        node.vertexArray.bind();
        glDrawArrays(GL_TRIANGLES, 0 ,node.numVertices);
    }
}
