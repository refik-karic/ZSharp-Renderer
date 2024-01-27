add_executable(SoftwareRenderer WIN32)

target_sources(SoftwareRenderer
    PRIVATE
	    Win32Main.cpp
)

target_include_directories(SoftwareRenderer SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/ZSharp/)
target_link_libraries(SoftwareRenderer PUBLIC ZSharp)

set(SoftwareRenderer_Compile_Options_Debug )

set(SoftwareRenderer_Compile_Options_Release )

set(SoftwareRenderer_Link_Options_Debug )

set(SoftwareRenderer_Link_Options_Release )

set(SoftwareRenderer_Preprocessor_Defines )

if(WIN32)
  list(APPEND SoftwareRenderer_Preprocessor_Defines 
    PLATFORM_WINDOWS
	_CRT_SECURE_NO_WARNINGS
	_HAS_EXCEPTIONS=0
	WIN32_LEAN_AND_MEAN
    PLATFORM_MAX_PATH=260
  )

  list(APPEND SoftwareRenderer_Compile_Options_Debug
  	/Od
	/Ob0
	/source-charset:utf-8
	/std:c++latest
	/D_UNICODE
	/GR-
	/MP
	/U_MBCS
	/W4
	/WX
    /Zi
	/diagnostics:caret
  )

  list(APPEND SoftwareRenderer_Compile_Options_Release
  	/O2
	/Ob2
	/source-charset:utf-8
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
    /Zi
	/diagnostics:caret
  )

  list(APPEND SoftwareRenderer_Link_Options_Debug
    /INCREMENTAL:NO
    /WX
  )

  list(APPEND SoftwareRenderer_Link_Options_Release
    /INCREMENTAL:NO
    /LTCG:NOSTATUS
    /WX
    /LTCG
    /DEBUG # Generate a PDB even in Release
  )

  # For high precision timing in main loop.
  target_link_libraries(SoftwareRenderer PRIVATE Winmm)
endif()

target_compile_definitions(SoftwareRenderer PRIVATE 
  ${SoftwareRenderer_Preprocessor_Defines}
)

if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Debug})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Debug})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release})

	else()
		message(FATAL_ERROR "Must specify -DCMAKE_BUILD_TYPE=Debug/Release when using Ninja.")
	endif()

else()
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
endif()
