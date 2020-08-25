#include "Texture2DArrayBuilder.h"
#include "Image.h"
#include "LoggerDefinition.h"
#include <cmath>
#include <stdexcept>

gl::Texture Texture2DArrayBuilder::build(std::uint32_t width, std::uint32_t height) const {
    gl::Texture result(gl::TextureBindingTarget::Texture2DArray, width, height, (std::uint32_t)mImages.size());
    int i = 0;
    for (const Image& image : mImages) {
        if (image.width() != width && image.height() != height) {
            warningstream << "An image\'s size isn\'tt match.\n" 
                 << "It should be: (" << width <<','<< height <<")\n"
                 << "But now it\'s ( " << image.width() <<',' << image.height() << ") .";
            result.subData(0, 0, i++, width, height, 1, flipImage(scaleImage(image, width, height)).data());
        }
        else {
            result.subData(0, 0, i++, width, height, 1, flipImage(image).data());
        }
    }
    return result;
}

std::uint32_t Texture2DArrayBuilder::append(Image&& image) noexcept {
    std::uint32_t result = (std::uint32_t)mImages.size();
    mImages.emplace_back(std::move(image));
    return result;
}

Image Texture2DArrayBuilder::scaleImage(const Image& image, std::uint32_t w, std::uint32_t h) {
    std::uint8_t* resultData = new std::uint8_t[w * h * 4];

    for (std::uint32_t dstY = 0; dstY < h; ++dstY){
        std::uint32_t srcY = (std::uint32_t)(std::floor((dstY + 0.5f) / h * image.height()));
        for (std::uint32_t dstX = 0; dstX < w; ++dstX) {
            std::uint32_t srcX = (std::uint32_t)(std::floor((dstX + 0.5f) / w * image.width()));
            for (int i = 0; i < 4; ++i) {
                resultData[(dstY * w + dstX) * 4 + i] = image.data()[(srcY * image.width() + srcX) * 4 + i];
            }
        }
    }
    
    return Image(w, h, resultData, 
        [](std::uint8_t* data) {
            delete[] data;
        });
}

Image Texture2DArrayBuilder::flipImage(const Image& image) {
    std::uint8_t* resultData = new std::uint8_t[image.width() * image.height() * 4];
    for (std::uint32_t y = 0; y < image.height(); ++y){
        for (std::uint32_t x = 0; x < image.width(); ++x) {
            for (int i = 0; i < 4; ++i) {
                resultData[(y * image.width() + x) * 4 + i] = image.data()[((image.height() - y - 1) * image.width() + x) * 4 + i];
            }
        }
    }
    return Image(image.width(), image.height(), resultData,
        [](std::uint8_t* data) {
            delete[] data;
        });
}
