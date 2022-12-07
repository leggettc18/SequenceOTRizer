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
    GIT_TAG 271c554f31cfb65a55e889bdeebf9a4577ab588c
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(libultraship)
FetchContent_Declare(
    OTRizer
    GIT_REPOSITORY https://github.com/leggettc18/OTRizer
    GIT_TAG 64e26f44974d58ac5d2c9244abf8e02c71dd0792
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(OTRizer)
FetchContent_Declare(
    ZeldaOTRizer
    GIT_REPOSITORY https://github.com/leggettc18/ZeldaOTRizer
    GIT_TAG 720c544b2cac02d2e04c9393c9498ff113dbc1b3
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
install(TARGETS SequenceOTRizer RUNTIME DESTINATION . COMPONENT Runtime)
set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. semi-colon delimited list")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_GROUPING IGNORE)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENT_ALL Runtime)
include(CPack)
