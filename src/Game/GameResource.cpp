#include "GameResource.h"
#include "../Util/Texture2DArrayBuilder.h"
#include "../Util/Image.h"
#include <stdexcept>

#include "../Util/LoggerDefinition.h"

void GameResource::load(GameResource::Side side) {
    for(auto& provider : mProviders) {
        std::uint16_t domain = mCommonResource.mBlockManager.genUnusedDomain();
        auto& blockManager = mCommonResource.mBlockManager;
        provider->provideBlocks(
            [&]() -> Block {
                return blockManager.genBlock();
            },
            domain
        );
    }
    if (side == GameResource::Side::Client) {
        Texture2DArrayBuilder t2db;
        BlockModelManager modelManager;
        for(auto& provider : mProviders) {
            provider->provideBlockModels(
                [&](const std::string& filename) {
                    std::uint32_t result = t2db.append(Image::fromFile("resources/image/" + filename));
                    return result;
                },
                [&](std::uint32_t id, std::unique_ptr<BlockModel>&& model) {
                    modelManager.registerModel(id >> 16, id & 0xffff, std::move(model));
                }
            );
        }
        mClientResource.reset(new GameResource::ClientResource({
            std::move(modelManager),
            t2db.build(16,16)
        }));
    }
}

void GameResource::appendProvider(std::unique_ptr<GameContentProvider>&& provider) noexcept {
    mProviders.emplace_back(std::move(provider));
}

const GameResource::CommonResource& GameResource::getCommonResource() const noexcept {
    return mCommonResource;
}

const GameResource::ClientResource& GameResource::getClientResource() const {
    if(!mClientResource) {
        throw std::runtime_error("No client resources has been initialized but now getClientResource() was called.");
    }
    return *mClientResource;
}
