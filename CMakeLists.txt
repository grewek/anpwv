cmake_minimum_required(VERSION 3.17)
project(anpwv 
    VERSION 0.1 
    DESCRIPTION "A noob plays with Vulkan" 
    LANGUAGES CXX
)

add_executable(anpwv
    src/anpwv.cpp
)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(anpwv glfw)
target_link_libraries(anpwv vulkan)
