cmake_minimum_required(VERSION 3.15)
project(ruckig-wasm LANGUAGES CXX)

# Ruckig library sources (no cloud client, no python)
set(RUCKIG_SOURCES
    ruckig/src/ruckig/brake.cpp
    ruckig/src/ruckig/position_first_step1.cpp
    ruckig/src/ruckig/position_first_step2.cpp
    ruckig/src/ruckig/position_second_step1.cpp
    ruckig/src/ruckig/position_second_step2.cpp
    ruckig/src/ruckig/position_third_step1.cpp
    ruckig/src/ruckig/position_third_step2.cpp
    ruckig/src/ruckig/velocity_second_step1.cpp
    ruckig/src/ruckig/velocity_second_step2.cpp
    ruckig/src/ruckig/velocity_third_step1.cpp
    ruckig/src/ruckig/velocity_third_step2.cpp
)

# Build as static library
add_library(ruckig STATIC ${RUCKIG_SOURCES})
target_compile_features(ruckig PUBLIC cxx_std_20)
target_include_directories(ruckig PUBLIC ruckig/include)

# WASM bindings
add_executable(ruckig_wasm bindings.cpp)
target_link_libraries(ruckig_wasm PRIVATE ruckig)
target_compile_features(ruckig_wasm PUBLIC cxx_std_20)

# Emscripten-specific flags
set_target_properties(ruckig_wasm PROPERTIES
    OUTPUT_NAME "ruckig"
    SUFFIX ".js"
)

target_link_options(ruckig_wasm PRIVATE
    --bind
    -sWASM=1
    -sMODULARIZE=1
    -sEXPORT_NAME=createRuckigModule
    -sALLOW_MEMORY_GROWTH=1
    -sENVIRONMENT=web,node
    -O2
)
