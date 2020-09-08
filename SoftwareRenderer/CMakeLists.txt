add_executable(SoftwareRenderer WIN32)

target_sources(SoftwareRenderer
    PRIVATE
        GDIWrapper.cpp
        GDIWrapper.h
        main.cpp
        WindowsHeadersWrapper.h
)

target_include_directories(SoftwareRenderer SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/ZSharp/)
target_link_libraries(SoftwareRenderer PUBLIC ZSharp)

set(SoftwareRenderer_Compile_Options 
	/W4
	/WX
	/sdl-
	/GR-
	/GS-
	/arch:AVX512
	/std:c++17
	/MP
)

target_compile_options(SoftwareRenderer PRIVATE 
	${SoftwareRenderer_Compile_Options}
)

set(RUNTIME_ASSETS)
get_property(RUNTIME_ASSETS
	TARGET ZSharp
	PROPERTY ZSHARP_ASSETS
)

add_custom_command(TARGET SoftwareRenderer
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${RUNTIME_ASSETS} ${SoftwareRendererV3_BINARY_DIR}
	COMMENT "Copying assets to build directory."
)
