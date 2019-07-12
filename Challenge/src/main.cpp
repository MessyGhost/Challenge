#include <iostream>
#include <chrono>
#include <thread>
#include "Game/Game.h"
#include "Game/GameResource.h"
#include "MagiCraft.h"
#include "Util/LoggerDefinition.h"
#include "Interface.h"

int main(int argc, char** argv){
    try {
		Interface::getInterface();
        auto startTime = std::chrono::system_clock::now();
        GameResource gamerc;
        gamerc.appendProvider(std::make_unique<MagiCraft>());
        gamerc.load(GameResource::Side::Client);
        infostream 
            << "Initialized successfully in " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() / 1000.0f
            << "s.";
        Game(gamerc).run();
        infostream << "Game stopped running.";
    }
    catch (std::exception& e) {
        errorstream 
            << "An uncaught exception was encountered.\n"
            << "Exception::what(): " << e.what() << '\n'
            << "Game will exit soon...";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 1;
    }
    return 0;
}
