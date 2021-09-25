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

set(SoftwareRenderer_Compile_Options 
	/source-charset:utf-8
	/std:c++latest
	/D_UNICODE
	/GR-
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

target_compile_definitions(SoftwareRenderer PRIVATE 
  FORCE_AVX512
  FORCE_SSE
)

target_compile_options(SoftwareRenderer PRIVATE 
	${SoftwareRenderer_Compile_Options}
)

target_link_options(SoftwareRenderer PUBLIC ${SoftwareRenderer_Link_Options})
