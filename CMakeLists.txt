cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(engine)
add_subdirectory(platform)
add_subdirectory(demo)

add_executable(PhysicsDemo main.cpp)
target_link_libraries(PhysicsDemo engine platform demo)
