add_library(demo STATIC
    Sandbox.cpp
)

target_include_directories(demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(demo PUBLIC engine)
