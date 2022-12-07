cmake_minimum_required(VERSION 3.24.0)
project(SequenceOTRizer VERSION 0.2)
option(BUILD_SHARED_LIBS "Build shared library" OFF)
if(BUILD_SHARED_LIBS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
include(FetchContent)
FetchContent_Declare(
    libultraship
    GIT_REPOSITORY https://github.com/leggettc18/libultraship
    GIT_TAG 74c9912cde751489799c921f2b849a5640ed84f3
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(libultraship)
FetchContent_Declare(
    OTRizer
    GIT_REPOSITORY https://github.com/leggettc18/OTRizer
    GIT_TAG c1d1ee3d88d8d688d960d7da457641c6ec6e3067
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(OTRizer)
FetchContent_Declare(
    ZeldaOTRizer
    GIT_REPOSITORY https://github.com/leggettc18/ZeldaOTRizer
    GIT_TAG fc7fe0b5029b12585260666a9e1d163183f74d0f
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(ZeldaOTRizer)
configure_file(config.h.in config.h)
add_library(SequenceOTRizer_compiler_flags INTERFACE)
target_compile_features(SequenceOTRizer_compiler_flags INTERFACE cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(SequenceOTRizer_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<BUILD_INTERFACE:/Od;/Wall;/Zi;>>"
)
add_executable(SequenceOTRizer main.cpp)
target_include_directories(SequenceOTRizer PUBLIC "${PROJECT_BINARY_DIR}")
find_package(ZeldaOTRizer)
target_link_libraries(SequenceOTRizer ZeldaOTRizer::ZeldaOTRizer SequenceOTRizer_compiler_flags)
install(TARGETS SequenceOTRizer RUNTIME DESTINATION .)
set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. semi-colon delimited list")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
