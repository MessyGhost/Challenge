#include <iostream>
#include "Game/Game.h"

int main(int argc, char** argv){
    try {
        Game().run();
    }
    catch (std::exception& e) {
        std::cerr 
            << "Error: "
            << e.what() <<std::flush;
        std::terminate();
    }
    return 0;
}
