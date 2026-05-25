cmake_minimum_required(VERSION 4.1)
project(Starfall)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/Dependencies.cmake)

add_compile_definitions(
        CLIENT_NAME="${PROJECT_NAME}"
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
file(GLOB_RECURSE DEPENDENCIES "deps/*.cpp" "deps/*.hpp")

add_library(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${DEPENDENCIES}
        src/Memories/Hooks/Core/ClientInstanceHook.cpp
        src/Memories/Hooks/Core/ClientInstanceHook.hpp
        src/SDK/Game/Core/ClientInstance.cpp
        src/SDK/Game/Core/ClientInstance.hpp
        src/Managers/HookManager.cpp
        src/Managers/HookManager.hpp
        src/Memories/Hooks/Windows/FreeLibraryAndExitHook.cpp
        src/Memories/Hooks/Windows/FreeLibraryAndExitHook.hpp
)

link_client_dependencies(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        deps
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WINDLL
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)