#include "Game.h"
#include "../Interface.h"
#include "../Graphic/ShaderProgram.h"
#include "../Graphic/VertexArray.h"
#include "../Graphic/Framebuffer.h"
#include "../Graphic/Camera.h"
#include "../Util/Texture2DArrayBuilder.h"
#include "../Util/Image.h"
#include <iostream>

Game::Game() noexcept {
}

void Game::run() {
    auto &interface = Interface::getInterface();
    gl::Framebuffer& screen = gl::Framebuffer::SCREEN;
    float clearColor[] = {0.1f, 0.2f, 0.3f, 1.0f};

    gl::VertexArray va;
    gl::VertexBuffer vb = gl::createVertexBuffer(
        {0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f}, gl::BufferUsage::StaticDraw),
         vb2 = gl::createVertexBuffer(
        {0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f
        }, gl::BufferUsage::StaticDraw);
    va.setAttribute(vb, 0, 2, gl::DataType::Float);
    va.setAttribute(vb2, 1, 3, gl::DataType::Float);
    va.enableAttribute(0);
    va.enableAttribute(1);
    va.bind();
    gl::ShaderProgram s = gl::loadShaderProgramFromFile("resource/shader/block.vert", "resource/shader/block.frag");
    Camera cmr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    cmr.setProjectionInfo(45.0f, 800.0f / 600.0f, 10.0f);
    s.setUniform("ProjectionMatrix", cmr.getProjectionMatrix());
    cmr.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));

    Texture2DArrayBuilder tb;
    tb.append(Image::fromFile("resource/image/grass.png"));
    auto textures = tb.build(16, 16);
    textures.bind();
    while (!interface.shouldLeave())
    {
        interface.handleWindowEvents();
        auto delta = interface.getRotationDelta();
        cmr.caculateRotation(glm::radians(delta));
        s.setUniform("ModelViewMatrix", cmr.getModelViewMatrix());
        s.use();
        textures.bind();
        screen.clearBuffer(gl::BufferType::ColorBuffer, clearColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        interface.swapWindow();
    }
}
