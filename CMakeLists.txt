cmake_minimum_required(VERSION 3.10)

project(ladus)

#add_compile_options(-std=c++17 -Wall -O2)
add_compile_options(-std=c++17 -Wall -Og -g)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED
    sdl2
    )

add_executable(ladus
    src/main.cpp
    src/surface.cpp
    src/util.cpp
    src/app.cpp
    src/world.cpp
    src/solid.cpp
    src/actor.cpp
    src/hero.cpp
    )

target_include_directories(ladus PRIVATE
    ${SDL_INCLUDE_DIRS}
    )

target_link_libraries(ladus
    ${SDL_LIBRARIES}
    )
