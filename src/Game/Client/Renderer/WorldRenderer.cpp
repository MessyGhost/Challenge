#include "WorldRenderer.h"
#include "../../../Util/LoggerDefinition.h"
#include <thread>
#include <functional>
#include <future>
#include <set>

WorldRenderer::WorldRenderer(
    const ChunkCache& chunkCache, 
    const BlockModelManager& blockModelManager, gl::ShaderProgram& shader)
    :mChunkCache(chunkCache), mBlockModelManager(blockModelManager), mBlockRenderingShader(shader)
{

}

void WorldRenderer::render(glm::dvec3 center, std::uint8_t radiusDistance) {
    auto& chunkCache = mChunkCache;
    auto& blockModelManager = mBlockModelManager;

    struct MeshData {
        std::vector<BlockModel::VertexFormat> models;
        struct Position {
            std::uint8_t x, y, z;
        };
        std::vector<Position> positionsInChunk;
        ChunkCache::ChunkPos chunkPos;
    };
    auto genChunkMesh = [&](const ChunkCache::ChunkPos& chunkPos) {
        MeshData result;

        auto& chunk = chunkCache.getChunk(chunkPos);
        std::uint8_t x, y, z;
        /*This function help to insert the vertices into the mesh data. */
        auto insertModel = [&](const std::vector<BlockModel::VertexFormat>& model){
            if(model.empty()) return;
            result.models.insert(result.models.end(), model.begin(), model.end());
            MeshData::Position position{
                x, y, z
            };
            for(auto i = model.size(); i > 0; --i) {
                result.positionsInChunk.emplace_back(position);
            }
        };
        /*This function help to cull face. */
        auto shouldRender = [&](const Chunk::BlockPos& blockPos, const BlockModel::BlockFace& face) -> bool {
            std::int16_t detectX = blockPos.x, detectY = blockPos.y, detectZ = blockPos.z;
            switch(face){
            case BlockModel::BlockFace::PositiveX:
            {
                detectX += 1;
                break;
            }
            case BlockModel::BlockFace::NegativeX:
            {
                detectX -= 1;
                break;
            }
            case BlockModel::BlockFace::PositiveY:
            {
                detectY += 1;
                break;
            }
            case BlockModel::BlockFace::NegativeY:
            {
                detectY -= 1;
                break;
            }
            case BlockModel::BlockFace::PositiveZ:
            {
                detectZ += 1;
                break;
            }
            case BlockModel::BlockFace::NegativeZ:
            {
                detectZ -= 1;
                break;
            }
            default:
                return false;
            }
            auto detectChunkPos = chunkPos;
            if(detectX >= Chunk::CHUNK_SIZE){detectX -= Chunk::CHUNK_SIZE;detectChunkPos.x += 1;}
            else if(detectX < 0){detectX += Chunk::CHUNK_SIZE;detectChunkPos.x -= 1;}

            if(detectY >= Chunk::CHUNK_SIZE){detectY -= Chunk::CHUNK_SIZE;detectChunkPos.y += 1;}
            else if(detectY < 0){detectY += Chunk::CHUNK_SIZE;detectChunkPos.y -= 1;}

            if(detectZ >= Chunk::CHUNK_SIZE){detectZ -= Chunk::CHUNK_SIZE;detectChunkPos.z += 1;}
            else if(detectZ < 0){detectZ += Chunk::CHUNK_SIZE;detectChunkPos.z -= 1;}
            try{return chunkCache.getChunk(detectChunkPos).getBlock({(std::uint8_t)detectX, (std::uint8_t)detectY, (std::uint8_t)detectZ}) == 0;}
            catch(std::out_of_range&){return true;}
        };
        for(x = 0; x < Chunk::CHUNK_SIZE; ++x)
         for(y = 0; y < Chunk::CHUNK_SIZE; ++y)
          for(z = 0; z < Chunk::CHUNK_SIZE; ++z) {
              auto block = chunk.getBlock({x, y, z});
              if(!block) continue;
              auto& model = blockModelManager.getModelByBlockId(block);
              for(int i = 0; i < BlockModel::FaceCount; ++i) {
                  if(shouldRender(Chunk::BlockPos{x, y, z}, (BlockModel::BlockFace)i)) {
                       insertModel(model.getCullableFace((BlockModel::BlockFace)i));
                  }
              }
              insertModel(model.getUncullableFace());
          }
        result.chunkPos = chunkPos;
        return result;
    };

    /*Target the chunk which requires to be rendered and make their mesh. */
    std::list<std::future<MeshData>> meshDataList;
    std::set<ChunkCache::ChunkPos> positionsOfRequiredRenderingChunks;
    std::int32_t 
        cx = (std::int32_t)std::floor(center.x / Chunk::CHUNK_SIZE),
        cy = (std::int32_t)std::floor(center.y / Chunk::CHUNK_SIZE),
        cz = (std::int32_t)std::floor(center.z / Chunk::CHUNK_SIZE);
    for (std::int32_t x = cx - radiusDistance; x <= cx + radiusDistance; ++x)
     for (std::int32_t y = cy - radiusDistance; y <= cy + radiusDistance; ++y)
      for (std::int32_t z = cz - radiusDistance; z <= cz + radiusDistance; ++z) {
          ChunkCache::ChunkPos pos{x, y, z};
          positionsOfRequiredRenderingChunks.emplace(pos);
          try{
           std::chrono::time_point<std::chrono::system_clock> lastChange;
           try{lastChange = mMeshes.at(pos).lastChange;}catch(std::out_of_range&){}
           if(mChunkCache.getChunk(pos).getLastWriting() >= lastChange)
            meshDataList.emplace_back(std::async(genChunkMesh, pos));
          }catch(std::out_of_range&){}
      }

    /*Target the mesh which is prepared for those non-existent chunk. */
    auto taskRecycleMeshNotRequired = std::async([&]() {
        std::list<decltype(mMeshes)::iterator> result;
        for(auto i = mMeshes.begin(); i != mMeshes.end(); ++i) {
            if(positionsOfRequiredRenderingChunks.find(i->first) == positionsOfRequiredRenderingChunks.end()) {
                result.emplace_back(i);
            }
        }
        return result;
    });
    
    /*Before render: upload the data */
    int numDataRequiredUpload = 0;
    for (auto& i : meshDataList) {
        ++numDataRequiredUpload;
        auto data = i.get();
        try {
            auto& mesh = mMeshes.at(data.chunkPos);
            if(
                  mesh.modelBuffer.size() < data.models.size() * BlockModel::VertexFormat::Size
                ||mesh.positionBuffer.size() < data.positionsInChunk.size() * sizeof(MeshData::Position)) {
                mMeshes.erase(data.chunkPos);
                throw std::out_of_range("");
            }
            mesh.modelBuffer.subData(data.models.size() * BlockModel::VertexFormat::Size, data.models.data());
            mesh.positionBuffer.subData(data.positionsInChunk.size() * sizeof(MeshData::Position), data.positionsInChunk.data());
            mesh.numVertices = data.models.size();
            mesh.lastChange = std::chrono::system_clock::now();
        }
        catch(std::out_of_range&) {
            mMeshes.emplace(
                std::piecewise_construct,
                std::make_tuple(data.chunkPos),
                std::make_tuple(genUnusedChunkMesh(
                    data.models.size() * BlockModel::VertexFormat::Size, data.models.data(),
                    data.positionsInChunk.size() * sizeof(MeshData::Position), data.positionsInChunk.data(),
                    (std::uint32_t)data.models.size())));
        }
    }
    if(numDataRequiredUpload)debugstream << numDataRequiredUpload << " mesh(es) was uploaded.";

    //Recycle mesh(only the main thread has the context to do this).
    auto chunksShouldBeRecycle = taskRecycleMeshNotRequired.get();
    int numRecycledChunks = 0;
    for(auto& i : chunksShouldBeRecycle) {
        recyleChunkMesh(std::move(i->second));
        mMeshes.erase(i);
        ++numRecycledChunks;
    }
    if(numRecycledChunks)debugstream << numRecycledChunks << "mesh(es) has been recycled.";

    //Real rendering
    for(auto& mesh : mMeshes) {
        mesh.second.vertexArray.bind();
        mBlockRenderingShader.setUniform("ChunkPosition", 
            glm::vec3(
                static_cast<float>((double)mesh.first.x * (double)Chunk::CHUNK_SIZE - center.x),
                static_cast<float>((double)mesh.first.y * (double)Chunk::CHUNK_SIZE - center.y),
                static_cast<float>((double)mesh.first.z * (double)Chunk::CHUNK_SIZE - center.z)
            )
        );
        mBlockRenderingShader.use();
        glDrawArrays(GL_TRIANGLES, 0 ,mesh.second.numVertices);
    }
}

WorldRenderer::ChunkMesh WorldRenderer::genUnusedChunkMesh(
        std::uint32_t modelBufferSize, const void* modelBufferData,
        std::uint32_t positionBufferSize, const void* positionBufferData, std::uint32_t numVertices) noexcept {
	for (auto i = mUnusedChunkMeshes.begin(); i != mUnusedChunkMeshes.end(); ++i) {
        if (i->modelBuffer.size() >= modelBufferSize && i->positionBuffer.size() >= positionBufferSize) {
            i->lastChange = std::chrono::system_clock::now();
            i->numVertices = numVertices;
            i->modelBuffer.subData(modelBufferSize, modelBufferData);
            i->positionBuffer.subData(positionBufferSize, positionBufferData);
			auto result = std::move(*i);
			mUnusedChunkMeshes.erase(i);
            debugstream << "ChunkMeshGen: Successfully return a recycled mesh.";
            return result;
        }
    }
    gl::VertexArray vertexArray;
    gl::VertexBuffer 
        modelBuffer(modelBufferSize, gl::BufferUsage::StreamDraw, modelBufferData), 
        positionBuffer(positionBufferSize, gl::BufferUsage::StreamDraw, positionBufferData);
    vertexArray.setAttribute(modelBuffer, 0, 3, gl::DataType::Float, false, BlockModel::VertexFormat::Size);
    vertexArray.setAttribute(modelBuffer, 1, 3, gl::DataType::Float, false, BlockModel::VertexFormat::Size, 
        BlockModel::VertexFormat::SizeOfPosition);
    vertexArray.setAttribute(positionBuffer, 2, 3, gl::DataType::UnsignedByte);
    for(int i = 0; i <= 2; ++i)vertexArray.enableAttribute(i);
    debugstream << "ChunkMeshGen: Return a new mesh.";
    return WorldRenderer::ChunkMesh{
        std::move(vertexArray),
        std::move(modelBuffer),
        std::move(positionBuffer),
        numVertices,
        std::chrono::system_clock::now()
    };
}

void WorldRenderer::recyleChunkMesh(WorldRenderer::ChunkMesh&& mesh) noexcept {
    mUnusedChunkMeshes.emplace_back(std::move(mesh));
}
