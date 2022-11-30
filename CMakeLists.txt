cmake_minimum_required(VERSION 3.16.0)
project(SequenceOTRizer VERSION 0.1)
configure_file(config.h.in config.h)
add_library(SequenceOTRizer_compiler_flags INTERFACE)
target_compile_features(SequenceOTRizer_compiler_flags INTERFACE cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set (BUILD_SHARED_LIBS OFF CACHE STRING "install/link shared isntead of static libs")
target_compile_options(SequenceOTRizer_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<BUILD_INTERFACE:-W3>>"
)
add_executable(SequenceOTRizer main.cpp)
target_include_directories(SequenceOTRizer PUBLIC "${PROJECT_BINARY_DIR}")
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    get_property(MSVC_RUNTIM_LIBRARY_DEFAULT TARGET SequenceOTRizer PROPERTY MSVC_RUNTIME_LIBRARY)
    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
            $<$<CONFIG:Debug>:
				MultiThreadedDebug
			>
			$<$<CONFIG:Release>:
				MultiThreaded
			>
			$<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
		string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
			$<$<CONFIG:Debug>:
				MultiThreadedDebug
			>
			$<$<CONFIG:Release>:
				MultiThreaded
			>
			$<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
		)
	endif()
	set_target_properties(SequenceOTRizer PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})
endif()
add_subdirectory(ZeldaOTRizer)
target_link_libraries(SequenceOTRizer ZeldaOTRizer SequenceOTRizer_compiler_flags)
install(TARGETS SequenceOTRizer RUNTIME DESTINATION .)
set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. semi-colon delimited list")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
