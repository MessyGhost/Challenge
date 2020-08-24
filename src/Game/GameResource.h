#pragma once
#include "../Util/Noncopyable.h"
#include "../Util/Nonmovable.h"
#include "GameContentProvider.h"
#include "Client/BlockModelManager.h"
#include "BlockManager.h"
#include "../Graphics/Texture.h"
#include <list>
#include <memory>

class GameResource
    :private Noncopyable, private Nonmovable
{
public:
    GameResource() = default;
    enum class Side {
        Client,
        Server
    };
    void load(Side side);
    
    void appendProvider(std::unique_ptr<GameContentProvider>&& provider) noexcept;
    struct CommonResource {
        BlockManager mBlockManager;
    };
    const CommonResource& getCommonResource() const noexcept;
    struct ClientResource {
        BlockModelManager mBlockModelManager;
        gl::Texture mBlockTexture;
    };
    const ClientResource& getClientResource() const;
private:
    std::list<std::unique_ptr<GameContentProvider>> mProviders;
    CommonResource mCommonResource;
    std::unique_ptr<ClientResource> mClientResource;
};
