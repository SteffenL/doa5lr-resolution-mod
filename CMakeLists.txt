cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(doa5lr_resolution_mod C CXX)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "4")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_WIX_PRODUCT_GUID "2CB1708A-15E3-4B13-934A-45D0B7D3CB5C")
set(CPACK_WIX_UPGRADE_GUID "4C9D1195-73C9-4DAA-B104-529E205A436F")

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MT>)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_library(minhook STATIC
    libs/minhook/src/buffer.c
    libs/minhook/src/hde/hde32.c
    libs/minhook/src/hook.c
    libs/minhook/src/trampoline.c
)
target_include_directories(minhook PUBLIC libs/minhook/include)

set(TARGET_NAME resmod)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.rc src/*.def ${CMAKE_CURRENT_BINARY_DIR}/src/*.rc)
message(${SOURCES})
add_library(${TARGET_NAME} SHARED ${SOURCES})
target_compile_definitions(${TARGET_NAME} PRIVATE MOD_VERSION="${CPACK_PACKAGE_VERSION}")
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
target_link_libraries(${TARGET_NAME} PRIVATE minhook)
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME d3d9)

set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
configure_file(src/version.rc.in src/version.rc @ONLY)

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION .)
install(FILES
    doc/end-users/mod_resolution.ini
    doc/end-users/mod_resolution_readme.txt
    DESTINATION .)

set(CPACK_GENERATOR "ZIP;WIX")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_NAME "DOA5LR Resolution Mod")
set(CPACK_PACKAGE_VENDOR "Steffen André Langnes")
set(CPACK_PACKAGE_FILE_NAME "Doa5lrResolutionMod-${CPACK_PACKAGE_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/installer/license.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SteffenL/doa5lr-resmod")
set(CPACK_WIX_PATCH_FILE "${CMAKE_SOURCE_DIR}/installer/patch.txt")
set(CPACK_WIX_TEMPLATE "${CMAKE_SOURCE_DIR}/installer/main.wxs")
set(CPACK_WIX_EXTRA_SOURCES
    "${CMAKE_SOURCE_DIR}/installer/ConfigDialog.wxs"
    "${CMAKE_SOURCE_DIR}/installer/ExitDialog.wxs"
    "${CMAKE_SOURCE_DIR}/installer/ModExistsDlg.wxs"
    "${CMAKE_SOURCE_DIR}/installer/WixUI_InstallDir.wxs"
)
set(CPACK_WIX_EXTENSIONS WixUtilExtension)
include(CPack)
