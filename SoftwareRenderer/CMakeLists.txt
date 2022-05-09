add_executable(SoftwareRenderer WIN32)

target_sources(SoftwareRenderer
    PRIVATE
        WindowsHeadersWrapper.h
	    ZSharpApplication.h

	    main.cpp
	    ZSharpApplication.cpp
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
  /LTCG:NOSTATUS
  /WX
)

set(SoftwareRenderer_Link_Options_Release 
  /INCREMENTAL:NO
  /LTCG:NOSTATUS
  /WX
  /LTCG
)

target_compile_definitions(SoftwareRenderer PRIVATE 
  FORCE_AVX512
  FORCE_SSE
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
