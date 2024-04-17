cmake_minimum_required(VERSION 3.15)

project(MUL2024)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
add_subdirectory(libs/SDL-release-2.30.2)

add_subdirectory(libs/geGL)


add_executable(${PROJECT_NAME} src/main.cpp src/videoReader.cpp src/videoReader.h src/timer.hpp)

find_package(OpenCV REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main geGL::geGL)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} opencv_highgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC src)

add_custom_target(run ./${PROJECT_NAME})
add_custom_target(steam steam)

if(NOT CLEAR_CMAKE_ROOT_DIR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR=".")
endif()
