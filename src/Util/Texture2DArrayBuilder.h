#pragma once
#include "../Game/Client/Graphics/Texture.h"
#include "Noncopyable.h"
#include "Nonmovable.h"
#include <vector>
#include <string>
#include <cstdint>

class Image;

class Texture2DArrayBuilder 
    :private Noncopyable, private Nonmovable
{
public:
    Texture2DArrayBuilder() = default;
    gl::Texture build(std::uint32_t width, std::uint32_t height) const;
    /*
        Return value: the index of the texture array.
     */
    std::uint32_t append(Image&& image) noexcept;
private:
    static Image scaleImage(const Image& image, std::uint32_t width, std::uint32_t height);
    static Image flipImage(const Image& image);
    std::vector<Image> mImages;
};
