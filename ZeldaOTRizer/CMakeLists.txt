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
add_library(ZeldaOTRizer STATIC Sequence.cpp)
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
	set_target_properties(ZeldaOTRizer PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})
endif()
target_include_directories(ZeldaOTRizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(OTRizer)
target_link_libraries(ZeldaOTRizer OTRizer ZeldaOTRizer_compiler_flags)