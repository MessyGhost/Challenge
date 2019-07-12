#pragma once
#include "Game/GameContentProvider.h"
#include <cstdint>

class MagiCraft 
    :public GameContentProvider
{
public:
    MagiCraft() = default;
    virtual void provideBlocks(
        const std::function<Block()>& newBlock,
        std::uint16_t domain
    ) override;
    virtual void provideBlockModels(
        const std::function<std::uint32_t(const std::string& filename)>& registerTexture,
        const std::function<void(std::uint32_t block, std::unique_ptr<BlockModel>&& model)>& registerModel
    ) override;
private:
    std::uint32_t BLOCK_GRASS;
};
