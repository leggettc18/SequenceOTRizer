cmake_minimum_required(VERSION 3.16.0)
project(SequenceOTRizer VERSION 0.1)
add_library(SequenceOTRizer_compiler_flags INTERFACE)
target_compile_features(SequenceOTRizer_compiler_flags INTERFACE cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(SequenceOTRizer_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wsahdow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<BUILD_INTERFACE:-W3>>"
)
add_executable(SequenceOTRizer main.cpp)
add_subdirectory(ZeldaOTRizer)
target_link_libraries(SequenceOTRizer ZeldaOTRizer SequenceOTRizer_compiler_flags)