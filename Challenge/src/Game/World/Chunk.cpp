#include "Chunk.h"
#include <stdexcept>
#include <sstream>

Chunk::Chunk() noexcept
    :mState(Chunk::State::Empty), m_pBlocks(nullptr), mLastWriting(std::chrono::system_clock::now())
{

}

Chunk::Chunk(const Chunk::BlockData& blocks) 
    :mState(Chunk::State::Full), m_pBlocks(new auto(blocks)), mLastWriting(std::chrono::system_clock::now())
{

}

Chunk::Chunk(Chunk::BlockData&& blocks)
    :mState(Chunk::State::Full), m_pBlocks(new auto(blocks)), mLastWriting(std::chrono::system_clock::now())
{

}

Chunk::Chunk(Chunk&& rhs) noexcept
    :m_pBlocks(std::move(rhs.m_pBlocks)), mState(rhs.mState), mLastWriting(std::chrono::system_clock::now())
{
    
}

std::uint32_t Chunk::getBlock(const Chunk::BlockPos& pos) const {
    if (!m_pBlocks) {
        throw std::runtime_error("Chunk has no data.");
    }
    else if (pos.x < 0 || pos.x >= CHUNK_SIZE || pos.y < 0 || pos.y >= CHUNK_SIZE || pos.z < 0 || pos.z>= CHUNK_SIZE) {
        std::stringstream buffer;
        buffer << "Cannot read the block(out of range): Chunk(" << pos.x <<", "<<pos.y<<", "<< pos.z << ").";
        throw std::out_of_range(buffer.str());
    }
    return (*m_pBlocks)[pos.x * Power<Chunk::CHUNK_SIZE, 2>::value + pos.y * Chunk::CHUNK_SIZE + pos.z];
}

void Chunk::setBlock(const Chunk::BlockPos& pos, std::uint32_t block) {
    if (!m_pBlocks) {
        throw std::runtime_error("Chunk has no data.");
    }
    else if (pos.x < 0 || pos.x >= CHUNK_SIZE || pos.y < 0 || pos.y >= CHUNK_SIZE || pos.z < 0 || pos.z>= CHUNK_SIZE) {
        std::stringstream buffer;
        buffer << "Cannot write the block(out of range): Chunk(" << pos.x <<", "<<pos.y<<", "<< pos.z << ").";
        throw std::out_of_range(buffer.str());
    }
    mLastWriting += (std::chrono::system_clock::now() - mLastWriting);
    (*m_pBlocks)[pos.x * Power<Chunk::CHUNK_SIZE, 2>::value + pos.y * Chunk::CHUNK_SIZE + pos.z] = block;
}

Chunk::State Chunk::getState() const noexcept {
    return mState;
}
void Chunk::setState(Chunk::State state) noexcept {
    mState = state;
}
