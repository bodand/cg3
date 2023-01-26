set(CMAKE_SYSTEM_PROCESSOR "x86_64")

if (ENV{MS_VS_PATH})
    set(VS_ROOT "$ENV{MS_VS_PATH}")
else ()
    set(VS_ROOT "C:/Program Files/Microsoft Visual Studio/2022/Community")
endif ()
set(clang_cl "${VS_ROOT}/VC/Tools/Llvm/x64/bin/clang-cl.exe")

set(CMAKE_C_COMPILER "${clang_cl}")
set(CMAKE_CXX_COMPILER "${clang_cl}")
set(CMAKE_C_FLAGS_Debug_INIT "-m64 /MDd")
set(CMAKE_C_FLAGS_Release_INIT "-m64 /MD")
set(CMAKE_CXX_FLAGS_Debug_INIT "-m64 /MDd")
set(CMAKE_CXX_FLAGS_Release_INIT "-m64 /MD")

# i cannot comment on this...
set(project_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
if (CMAKE_CURRENT_SOURCE_DIR MATCHES [[(.+)/_build-]])
    set(project_root_path "${CMAKE_MATCH_1}")
endif ()

include("${project_root_path}/vcpkg/scripts/buildsystems/vcpkg.cmake")
