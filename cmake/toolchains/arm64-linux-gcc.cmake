set(CMAKE_SYSTEM_PROCESSOR "arm64")

if (DEFINED ENV{CG_CC_EXE})
    set(CMAKE_C_COMPILER "$ENV{CG_CC_EXE}")
else ()
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
endif ()

if (DEFINED ENV{CG_CXX_EXE})
    set(CMAKE_C_COMPILER "$ENV{CG_CXX_EXE}")
else ()
    set(CMłAKE_CXX_COMPILER "/usr/bin/g++")
endif ()

set(CMAKE_C_FLAGS_INIT "-march=armv8-a")
set(CMAKE_CXX_FLAGS_INIT "-march=armv8-a")
