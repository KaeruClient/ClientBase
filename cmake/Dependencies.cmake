include(FetchContent)
set(FMT_MODULE OFF CACHE BOOL "Disable fmt module" FORCE)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
# SafetyHook
FetchContent_Declare(
        safetyhook
        GIT_REPOSITORY https://github.com/cursey/safetyhook.git
        GIT_TAG main
        GIT_SUBMODULES ""
)
# LibHat
FetchContent_Declare(
        libhat
        GIT_REPOSITORY https://github.com/BasedInc/libhat.git
        GIT_TAG v0.9.0
)
# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
# EnTT
FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG fe8d7d78c4823e8a66a050bf86f5c6318cf76ce7 # Minecraft 1.26.3
)
# fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        main
)

# DirectX Headers
FetchContent_Declare(
        DirectXHeaders
        GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
        GIT_TAG v1.606.4
)

# nlohmann-json
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(
        safetyhook
        libhat
        glm
        EnTT
        fmt
        DirectXHeaders
        json
)

macro(link_client_dependencies TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE
            safetyhook::safetyhook
            libhat::libhat
            glm::glm
            EnTT::EnTT
            fmt
            Microsoft::DirectX-Headers
            nlohmann_json::nlohmann_json
            d3d12.lib
            dxgi.lib
    )
endmacro()