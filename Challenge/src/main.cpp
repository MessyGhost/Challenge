#include <iostream>
#include "Graphic/VertexArray.h"
#include "Graphic/ShaderProgram.h"
#include "Interface.h"

int main(int argc, char** argv){
    try {
        auto& interface = Interface::getInterface();
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gl::VertexArray va;
        gl::VertexBuffer vb = gl::createVertexBuffer({0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}, gl::BufferUsage::StaticDraw);
        va.setAttribute(vb, 0, 2, gl::DataType::Float);
        va.enableAttribute(0);
        va.bind();
        gl::ShaderProgram s = gl::loadShaderProgramFromFile("resource/shader/block.vert", "resource/shader/block.frag");
        s.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        interface.swapWindow();
        while (!interface.shouldLeave()){
            interface.handleWindowEvents();
        }
    }
    catch (std::exception& e) {
        std::cerr 
            << "Error: "
            << e.what() <<std::flush;
        std::terminate();
    }
    return 0;
}
