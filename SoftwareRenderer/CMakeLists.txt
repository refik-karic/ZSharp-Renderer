add_executable(SoftwareRenderer WIN32)

target_sources(SoftwareRenderer
    PRIVATE
	    Win32Main.cpp
)

target_include_directories(SoftwareRenderer SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/ZSharp/)
target_link_libraries(SoftwareRenderer PUBLIC ZSharp)

set(SoftwareRenderer_Compile_Options_Debug 
	/source-charset:utf-8
	/std:c17
	/std:c++latest
	/D_UNICODE
	/GR-
	/MP
	/U_MBCS
	/W4
	/WX
)

set(SoftwareRenderer_Compile_Options_Release 
	/source-charset:utf-8
	/std:c17
	/std:c++latest
	/D_UNICODE
	/GR-
	/MP
	/U_MBCS
	/W4
	/WX
	/sdl-
	/GL
	/GS-
)

set(SoftwareRenderer_Link_Options_Debug 
  /INCREMENTAL:NO
  /WX
)

set(SoftwareRenderer_Link_Options_Release 
  /INCREMENTAL:NO
  /LTCG:NOSTATUS
  /WX
  /LTCG
)

set(SoftwareRenderer_Preprocessor_Defines )

if(WIN32)
  list(APPEND SoftwareRenderer_Preprocessor_Defines 
    PLATFORM_WINDOWS
	_CRT_SECURE_NO_WARNINGS
	_HAS_EXCEPTIONS=0
	WIN32_LEAN_AND_MEAN
    PLATFORM_MAX_PATH=260
  )
endif(WIN32)

target_compile_definitions(SoftwareRenderer PRIVATE 
  ${SoftwareRenderer_Preprocessor_Defines}
)

target_compile_options(SoftwareRenderer PRIVATE 
	"$<$<CONFIG:DEBUG>:${SoftwareRenderer_Compile_Options_Debug}>"
)

target_compile_options(SoftwareRenderer PRIVATE 
	"$<$<CONFIG:RELEASE>:${SoftwareRenderer_Compile_Options_Release}>"
)

target_link_options(SoftwareRenderer PUBLIC 
	"$<$<CONFIG:DEBUG>:${SoftwareRenderer_Link_Options_Debug}>"
)

target_link_options(SoftwareRenderer PUBLIC 
	"$<$<CONFIG:RELEASE>:${SoftwareRenderer_Link_Options_Release}>"
)
