#include "Texture2DArrayBuilder.h"
#include "Image.h"
#include <cmath>

gl::Texture Texture2DArrayBuilder::build(std::uint32_t width, std::uint32_t height) const noexcept {
    gl::Texture result(gl::TextureBindingTarget::Texture2DArray, width, height, mImages.size());
    int i = 0;
    for (const Image& image : mImages) {
        if (image.width() != width && image.height() != height) {
            result.subData(0, 0, i++, width, height, 1, scaleImage(image, width, height).data());
        }
        else {
            result.subData(0, 0, i++, width, height, 1, image.data());
        }
    }
    return result;
}

std::uint32_t Texture2DArrayBuilder::append(Image&& image) noexcept {
    std::uint32_t result = mImages.size();
    mImages.push_back(std::move(image));
    return result;
}

#include <iostream>
Image Texture2DArrayBuilder::scaleImage(const Image& image, std::uint32_t w, std::uint32_t h) noexcept {
    std::uint8_t* resultData = new std::uint8_t[w * h * 4];

    for (std::uint32_t dstY = 0; dstY < h; ++dstY){
        std::uint32_t srcY = std::floor((dstY + 0.5f) / h * image.height());
        srcY = srcY >= image.height() ? srcY - 1 : srcY;
        for (std::uint32_t dstX = 0; dstX < w; ++dstX) {
            std::uint32_t srcX = std::floor((dstX + 0.5f) / w * image.width());
            srcX = srcX >= image.width() ? srcX - 1 : srcX;
            std::cout << dstY * w + dstX << ": " << dstX << "," << dstY << " <- "
                    << srcY * image.width() + srcX << ":" << srcX << "," << srcY <<std::endl;
            for (int i = 0; i < 4; ++i) {
                resultData[(dstY * w + dstX) * 4 + i] = image.data()[(srcY * image.width() + srcX) * 4 + i];
            }
        }
    }
    
    return Image(w, h, resultData, 
        [](uint8_t* data) noexcept {
            delete data;
        });
}
