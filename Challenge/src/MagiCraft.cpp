#include "MagiCraft.h"
#include "Game/Client/Renderer/Model/ModelCube.h"

void MagiCraft::provideBlocks(const std::function<Block()>& newBlock, std::uint16_t domain) {
    BLOCK_GRASS = newBlock().setName("Grass").registerBlock(domain);
}

void MagiCraft::provideBlockModels(
        const std::function<std::uint32_t(const std::string& filename)>& registerTexture,
        const std::function<void(std::uint32_t block, std::unique_ptr<BlockModel>&& model)>& registerModel
    )
{
    std::uint32_t 
        grassTop = registerTexture("grass_top.png"),
        grassBottom = registerTexture("grass_bottom.png"),
        grassSide = registerTexture("grass_side.png");
    registerModel(BLOCK_GRASS, std::make_unique<ModelCube>(grassSide, grassSide, grassTop, grassBottom, grassSide, grassSide));
}
