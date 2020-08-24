#pragma once
#include <functional>
#include <cstdint>
#include <string>
#include <memory>
#include "Block.h"
#include "Client/Renderer/BlockModel.h"

class GameContentProvider{
public:
    GameContentProvider() noexcept = default;
    GameContentProvider(const GameContentProvider&) = default;
    GameContentProvider(GameContentProvider&&) = default;
    virtual ~GameContentProvider() = default;
    virtual void provideBlocks(
        const std::function<Block()>& newBlock,
        std::uint16_t domain
    ) = 0;
    virtual void provideBlockModels(
        const std::function<std::uint32_t(const std::string& filename)>& registerTexture,
        const std::function<void(std::uint32_t block, std::unique_ptr<BlockModel>&& model)>& registerModel
    ) = 0;
};
