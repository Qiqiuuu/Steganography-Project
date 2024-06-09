cmake_minimum_required(VERSION 3.27)
project(PJCProject)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(PJCProject
        main.cpp
        graphic/graphic.cpp
        console/console.cpp
        graphic/BMP.hpp
        graphic/ImageAbstract.hpp
        graphic/PPM.hpp
        graphic/TGA.hpp)

target_link_libraries(PJCProject fmt)
