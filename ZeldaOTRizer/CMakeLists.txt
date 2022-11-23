cmake_minimum_required(VERSION 3.16.0)
project(ZeldaOTRizer VERSION 0.1)
add_library(ZeldaOTRizer_compiler_flags INTERFACE)
target_compile_features(ZeldaOTRizer_compiler_flags INTERFACE cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(ZeldaOTRizer_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wsahdow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<BUILD_INTERFACE:-W3>>"
)
add_library(ZeldaOTRizer Sequence.cpp)
target_include_directories(ZeldaOTRizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(OTRizer)
target_link_libraries(ZeldaOTRizer OTRizer ZeldaOTRizer_compiler_flags)