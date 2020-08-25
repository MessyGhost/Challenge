#include "Game.h"
#include "../Interface.h"
#include "Client/Graphics/ShaderProgram.h"
#include "Client/Graphics/Framebuffer.h"
#include "../Util/LoggerDefinition.h"
#include "Client/Renderer/WorldRenderer.h"
#include "World/World.h"
#include <iostream>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const GameResource& gamerc) noexcept 
    :mGameResource(gamerc)
{
}

void Game::run() {
    infostream << "Game::run() was called, a client will be launched soon...";
    auto &interface = Interface::getInterface();
    interface.present();
    interface.setMouseVisible(false);
    gl::enable(gl::Cap::DepthTest);
    gl::Framebuffer& screen = gl::Framebuffer::SCREEN;
    float clearColor[] = {0.1f, 0.2f, 0.3f, 1.0f};
    float clearDepth = 1.0f;

    gl::ShaderProgram s = gl::loadShaderProgramFromFile("resources/shader/block.vert", "resources/shader/block.frag");
    Interface::MoveIntent moveIntent;

    World world;
    for(std::int32_t x = -2; x <= 2; ++x)
    for(std::int32_t z = -2; z <= 2; ++z)
    world.loadChunk(ChunkCache::ChunkPos{x, 0, z});
    
    WorldRenderer wr(world.getChunkCache(), mGameResource.getClientResource().mBlockModelManager, s);

    glm::dvec3 playerPosition(0.0, 48.0, 0.0);
    glm::dvec2 playerRotation(0.0, 0.0);
    s.setUniform("ProjectionMatrix", glm::perspective(glm::radians(75.0f), 800.0f / 600.0f, 0.1f, 100.0f));

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    double delta;
    while (!interface.shouldLeave())
    {
        delta = 
            (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count()
            / 1000.0f;
        start = std::chrono::system_clock::now();
        interface.handleWindowEvents();

        /*Render */
        s.setUniform("ModelViewMatrix", glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(
                glm::cos(playerRotation.y) * glm::cos(playerRotation.x),
                glm::sin(playerRotation.x),
                glm::sin(playerRotation.y) * glm::cos(playerRotation.x)
            ),
            glm::vec3(0.0f, 1.0f, 0.0f)
        ));
        mGameResource.getClientResource().mBlockTexture.bind();
        screen.clearBuffer(gl::BufferType::ColorBuffer, clearColor);
        screen.clearBuffer(gl::BufferType::DepthBuffer, &clearDepth);
        wr.render(playerPosition, 4);
        interface.swapWindow();

        /*Update */
        auto rotationDelta = interface.getRotationDelta();
        playerRotation += glm::radians(rotationDelta);
        playerRotation.y += 
            playerRotation.y > 2.0 * glm::pi<double>() ? -glm::pi<double>() :
            playerRotation.y < 0.0 ? 2.0 * glm::pi<double>() : 0.0;
        playerRotation.x = 
            playerRotation.x > glm::half_pi<double>() ? glm::half_pi<double>() :
            playerRotation.x < -glm::half_pi<double>() ? -glm::half_pi<double>() : playerRotation.x;
        interface.getMoveIntent(moveIntent);
        glm::dvec3 offset(
            glm::cos(moveIntent.yaw + playerRotation.y) * moveIntent.distanceOnPlane, 
            (double)moveIntent.deltaOnVertical,
            glm::sin(moveIntent.yaw + playerRotation.y) * moveIntent.distanceOnPlane);
        playerPosition += offset * delta * 2.0;
    }
}
