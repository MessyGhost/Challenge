cmake_minimum_required(VERSION 3.15)
project(Challenge)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/./src)

aux_source_directory(${SOURCE_DIR}/ source)

add_executable(challenge ${source})

set(CMAKE_CXX_STANDARD 14)


target_include_directories(challenge PUBLIC ${CMAKE_SOURCE_DIR}/./3rd-parties/includes)
target_link_directories(challenge PUBLIC ${CMAKE_SOURCE_DIR}/./3rd-parties/libraries)
target_link_libraries(challenge glew32 glew32s SDL2 SDL2main SDL2test)

if(WIN32)
target_link_libraries(challenge opengl32)
endif()
