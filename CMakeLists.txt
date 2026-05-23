cmake_minimum_required(VERSION 4.1)
project(Starfall)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/Dependencies.cmake)

add_compile_definitions(
        CLIENT_NAME="${PROJECT_NAME}"
        TARGET_PROCESS="Minecraft.Windows.exe"
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(${PROJECT_NAME} SHARED
        ${SOURCES}
)

link_client_dependencies(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        src
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WINDLL
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)