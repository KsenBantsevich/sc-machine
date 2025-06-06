cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
project(sc-machine VERSION 0.10.4 LANGUAGES C CXX)
message(STATUS "Current project version: ${CMAKE_PROJECT_VERSION}")
site_name(www.ostis.net)
cmake_policy(SET CMP0048 NEW)

set(SC_MACHINE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${SC_MACHINE_ROOT}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Binary output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "Library output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

if(DEFINED "${SC_EXTENSIONS_DIRECTORY}")
    message(STATUS "Provided extensions directory: ${SC_EXTENSIONS_DIRECTORY}")
    file(MAKE_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
else()
    set(SC_EXTENSIONS_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/extensions)
    file(MAKE_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
    message(STATUS "No extensions directory provided. Defaulting to: ${SC_EXTENSIONS_DIRECTORY}")
endif()

set(SC_MACHINE_THIRDPARTY_PATH "${SC_MACHINE_ROOT}/thirdparty")

option(SC_BUILD_TESTS "Flag to build unit tests" OFF)
option(SC_BUILD_BENCH "Flag to build benchmark" OFF)

set(SC_FILE_MEMORY "Dictionary" CACHE STRING "sc-fs-storage type")
option(SC_OPTIMIZE_SEARCHING_INCOMING_CONNECTORS_FROM_STRUCTURES "Flag to optimize searching incoming sc-connectors from sc-structures" ON)

include(${SC_MACHINE_ROOT}/macro/macros.cmake)
parse_project_version()

code_coverage(SC_COVERAGE "Flag to generate coverage report" OFF "-g -O0 --coverage")
option(SC_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)
sanitizer(SC_USE_SANITIZER "Build with specified sanitizer" "none")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

option(AUTO_CCACHE "Use ccache to speed up rebuilds" ON)

if(${SC_BUILD_TESTS})
    include(GoogleTest)
    find_package(GTest REQUIRED)
    include(${CMAKE_MODULE_PATH}/tests.cmake)
endif()

if(${SC_BUILD_BENCH})
    find_package(benchmark REQUIRED)
endif()

if(${SC_CLANG_FORMAT_CODE})
    include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
    if (${SC_USE_SANITIZER} STREQUAL "none" AND NOT ${SC_COVERAGE})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    if(${SC_USE_SANITIZER} STREQUAL "none" AND NOT ${SC_COVERAGE})
	    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") ## Optimize
    endif()
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s") ## Strip binary
elseif (NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM AND ${AUTO_CCACHE})
    message(STATUS "Using ${CCACHE_PROGRAM} as compiler launcher")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")  # requires at least CMake 3.9 to be any use
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

if(${SC_FILE_MEMORY} STREQUAL "Dictionary")
    message("Build with sc-dictionary fs-memory")
    add_definitions(-DSC_DICTIONARY_FS_MEMORY)
else()
    message(FATAL_ERROR "File memory type is not set up, CMake will exit.")
endif()

if(${SC_OPTIMIZE_SEARCHING_INCOMING_CONNECTORS_FROM_STRUCTURES})
    message("Build optimized checking local user permissions")
    add_definitions(-DSC_OPTIMIZE_SEARCHING_INCOMING_CONNECTORS_FROM_STRUCTURES)
endif()

include(CTest)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG)

include(GNUInstallDirs)

message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, path: ${CMAKE_CXX_COMPILER}")

# Set up dynamic linking paths for our own libraries
# For macOS, use @loader_path instead of $ORIGIN
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path;@loader_path/../lib;")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib;")
endif()

# Always use full RPATH
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include(${CMAKE_MODULE_PATH}/find_glib.cmake)

add_subdirectory(${SC_MACHINE_ROOT}/thirdparty)
add_subdirectory(${SC_MACHINE_ROOT}/sc-memory)
add_subdirectory(${SC_MACHINE_ROOT}/sc-kpm)
add_subdirectory(${SC_MACHINE_ROOT}/sc-tools)

include(${CMAKE_MODULE_PATH}/install.cmake)
