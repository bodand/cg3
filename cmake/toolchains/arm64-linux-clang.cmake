set(CMAKE_SYSTEM_PROCESSOR "arm64")
cmake_policy(PUSH)
cmake_policy(SET CMP0054 NEW)

if (DEFINED ENV{CG_CC_EXE})
    set(CMAKE_C_COMPILER "$ENV{CG_CC_EXE}")
else ()
    set(CMAKE_C_COMPILER "/usr/bin/clang")
endif ()

if (DEFINED ENV{CG_CXX_EXE})
    set(CMAKE_CXX_COMPILER "$ENV{CG_CXX_EXE}")
else ()
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif ()

set(CMAKE_C_FLAGS_INIT "-march=armv8-a")
set(CMAKE_CXX_FLAGS_INIT "-march=armv8-a")

set(project_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
if (CMAKE_CURRENT_SOURCE_DIR MATCHES [[(.+)/_build-]])
    set(project_root_path "${CMAKE_MATCH_1}")
endif ()

cmake_policy(POP)

include("${project_root_path}/vcpkg/scripts/buildsystems/vcpkg.cmake")
