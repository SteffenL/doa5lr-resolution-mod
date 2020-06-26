cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(doa5lr_resolution_mod C CXX)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MT>)
endif()

add_library(minhook STATIC
    libs/minhook/src/buffer.c
    libs/minhook/src/hde/hde32.c
    libs/minhook/src/hook.c
    libs/minhook/src/trampoline.c
)
target_include_directories(minhook PUBLIC libs/minhook/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.rc src/*.def)
add_library(resmod SHARED ${SOURCES})
target_compile_features(resmod PUBLIC cxx_std_17)
target_link_libraries(resmod PRIVATE minhook)
set_target_properties(resmod PROPERTIES OUTPUT_NAME d3d9)
