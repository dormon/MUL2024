cmake_minimum_required(VERSION 3.15)

project(MUL2024)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
add_subdirectory(libs/SDL-release-2.30.2)


add_executable(${PROJECT_NAME} src/main.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main)
