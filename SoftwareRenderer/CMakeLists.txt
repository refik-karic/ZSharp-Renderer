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
	/favor:INTEL64
	/fp:fast
	/guard:cf-
	/permissive-
	/sdl-
	/source-charset:utf-8
	/std:c++latest
	/D_UNICODE
	/Gw
	/Gy
	/GL
	/GR-
	/GS-
	/MP
	/U_MBCS
	/W4
	/WX
)

set(SoftwareRenderer_Link_Options 
  /INCREMENTAL:NO
	/LTCG:NOSTATUS
	/WX
)

target_compile_options(SoftwareRenderer PRIVATE 
	${SoftwareRenderer_Compile_Options}
)

target_link_options(SoftwareRenderer PUBLIC ${SoftwareRenderer_Link_Options})

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
