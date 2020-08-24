cmake_minimum_required(VERSION 3.15)
project(Challenge)

aux_source_directory(${CMAKE_SOURCE_DIR}/./src/ source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Game/ source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Game/Client source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Game/Client/Renderer source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Game/Client/Renderer/Model source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Game/World source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Graphics/ source)
aux_source_directory(${CMAKE_SOURCE_DIR}/./src/Util/ source)

add_executable(challenge ${source})

target_include_directories(challenge PUBLIC ${CMAKE_SOURCE_DIR}/./3rd-parties/includes)
target_link_directories(challenge PUBLIC ${CMAKE_SOURCE_DIR}/./3rd-parties/libraries)
target_link_libraries(challenge glew32 glew32s SDL2 SDL2main SDL2test)

if(WIN32)
target_link_libraries(challenge opengl32)
endif()
