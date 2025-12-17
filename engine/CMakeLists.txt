add_library(engine STATIC
    math/Vector2.cpp
    physics/RigidBody.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})