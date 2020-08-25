#pragma once
#include <GL/glew.h>
#include <cstdint>

namespace gl {
    enum class DataType {
        Half = GL_HALF_FLOAT,
        Float = GL_FLOAT,
        Double = GL_DOUBLE,
        Byte = GL_BYTE,
        UnsignedByte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Int = GL_INT,
        UnsignedInt = GL_UNSIGNED_INT,
        Long = GL_INT64_ARB,
        UnsignedLong = GL_UNSIGNED_INT64_ARB
    };

    enum class Cap : GLenum {
        DepthTest = GL_DEPTH_TEST
    }; 

    inline void enable(Cap cap) noexcept {
        glEnable(static_cast<GLenum>(cap));
    }
}
