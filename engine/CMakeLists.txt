add_library(engine STATIC
    math/Vector2.cpp
    physics/RigidBody.cpp
    physics/PhysicsWorld.cpp
    collision/Collision.cpp
    collision/CollisionResolver.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})