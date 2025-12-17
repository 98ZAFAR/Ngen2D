find_package(SDL2 REQUIRED)

add_library(platform STATIC
    SDLApp.cpp
)

target_include_directories(platform PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(platform PUBLIC ${SDL2_LIBRARIES})