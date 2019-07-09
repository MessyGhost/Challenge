#include "Game.h"
#include "../Interface.h"
#include "../Graphic/ShaderProgram.h"
#include "../Graphic/VertexArray.h"
#include "../Graphic/Framebuffer.h"
#include "../Graphic/Camera.h"
#include "../Util/Texture2DArrayBuilder.h"
#include "../Util/Image.h"
#include "Renderer/Model/ModelCube.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Game::Game() noexcept {
}

void Game::run() {
    auto &interface = Interface::getInterface();
    interface.setMouseVisible(false);
    gl::enable(gl::Cap::DepthTest);
    gl::Framebuffer& screen = gl::Framebuffer::SCREEN;
    float clearColor[] = {0.1f, 0.2f, 0.3f, 1.0f};
    float clearDepth = 1.0f;

    Texture2DArrayBuilder tb;
    std::uint32_t 
        side = tb.append(Image::fromFile("resource/image/grass_side.png")),
        top = tb.append(Image::fromFile("resource/image/grass_top.png")),
        bottom = tb.append(Image::fromFile("resource/image/grass_bottom.png"));
    auto textures = tb.build(16, 16);
    textures.bind();

    ModelCube cube(side, side, top ,bottom, side, side);
    std::vector<BlockModel::VertexFormat> vertices;
    for (int i = 0; i < BlockModel::BlockFace::FaceCount; ++i) {
        const auto& model = cube.getCullableFace((BlockModel::BlockFace)i);
        vertices.insert(vertices.end(), model.begin(), model.end());
    }
    gl::VertexArray va;
    gl::VertexBuffer vb(vertices.size() * sizeof(BlockModel::VertexFormat), gl::BufferUsage::StaticDraw,
                vertices.data());
    va.setAttribute(vb, 0, 3, gl::DataType::Float, false, 6 * sizeof(float));
    va.setAttribute(vb, 1, 3, gl::DataType::Float, false, 6 * sizeof(float), 3 * sizeof(float));
    va.enableAttribute(0);
    va.enableAttribute(1);
    va.bind();

    Camera cmr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    cmr.setProjectionInfo(glm::radians(45.0f), 800.0f / 600.0f, 100.0f);

    gl::ShaderProgram s = gl::loadShaderProgramFromFile("resource/shader/block.vert", "resource/shader/block.frag");
    s.setUniform("ProjectionMatrix", cmr.getProjectionMatrix());
    Interface::MoveIntent moveIntent;

    cmr.setPosition(glm::vec3(3.0f, 3.0f, 3.0f));
    cmr.setRotation(glm::radians(glm::vec2(-45.0f ,225.0f)));
    while (!interface.shouldLeave())
    {
        interface.handleWindowEvents();

        auto delta = interface.getRotationDelta();
        cmr.calculateRotation(glm::radians(delta));

        auto rotation = cmr.getRotation();
        interface.getMoveIntent(moveIntent);
        glm::vec3 offset(
            glm::cos(moveIntent.yaw + rotation.y) * moveIntent.distanceOnPlane, 
            moveIntent.deltaOnVertical,
            glm::sin(moveIntent.yaw + rotation.y) * moveIntent.distanceOnPlane);
        cmr.calculatePosition(offset / 5.0f);
        auto pos = cmr.getPosition();

        s.setUniform("ModelViewMatrix", cmr.getModelViewMatrix());
        s.use();
        
        textures.bind();

        screen.clearBuffer(gl::BufferType::ColorBuffer, clearColor);
        screen.clearBuffer(gl::BufferType::DepthBuffer, &clearDepth);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        interface.swapWindow();
    }
}
