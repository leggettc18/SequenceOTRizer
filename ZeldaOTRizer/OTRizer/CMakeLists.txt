cmake_minimum_required(VERSION 3.16.0)
project(OTRizer VERSION 0.1)
add_library(OTRizer_compiler_flags INTERFACE)
target_compile_features(OTRizer_compiler_flags INTERFACE cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(OTRizer_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wsahdow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<BUILD_INTERFACE:-W3>>"
)
add_library(OTRizer Resource.cpp)
target_include_directories(OTRizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
include(FetchContent)
FetchContent_Declare(
    libultraship
    GIT_REPOSITORY https://github.com/Kenix3/libultraship
    GIT_TAG 9e98291cecce150aa9e04bd9a0a6d229d9474292
)
FetchContent_MakeAvailable(libultraship)
target_link_libraries(OTRizer PUBLIC OTRizer_compiler_flags libultraship)