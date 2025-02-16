cmake_minimum_required(VERSION 3.28)

# It's important this comes before the project() command
set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

project(sdl_wasm_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten-specific settings
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    # Add SDL2 flags for both compilation and linking
    add_compile_options(-sUSE_SDL=2)
    add_link_options(
        -sUSE_SDL=2
        -sWASM=1
        -sFULL_ES3=1
        -sUSE_WEBGL2=1
        -sALLOW_MEMORY_GROWTH=1
        -sNO_EXIT_RUNTIME=1
        -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
        -sFORCE_FILESYSTEM=1
        -sEXPORT_ALL=1
        # Enable C++ exceptions
        -fexceptions
        # Enable C++ standard library features
        -sNO_DISABLE_EXCEPTION_CATCHING
    )
endif()

add_executable(sdl_wasm_project main.cpp)

include_directories(
    "$ENV{EMSDK}/upstream/emscripten/cache/sysroot/include"
)