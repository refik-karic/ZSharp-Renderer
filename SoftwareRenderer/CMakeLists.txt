add_executable(SoftwareRenderer WIN32)

set(SoftwareRenderer_Header_Files )
set(SoftwareRenderer_Source_Files 
  Win32Main.cpp
)

target_sources(SoftwareRenderer
    PRIVATE
	    ${SoftwareRenderer_Header_Files}
	    ${SoftwareRenderer_Source_Files}
)

target_include_directories(SoftwareRenderer SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/ZSharp/)
target_link_libraries(SoftwareRenderer PUBLIC ZSharp)

set(SoftwareRenderer_Compile_Options_Debug )

set(SoftwareRenderer_Compile_Options_Release )

set(SoftwareRenderer_Link_Options_Debug )

set(SoftwareRenderer_Link_Options_MinSizeRel )

set(SoftwareRenderer_Link_Options_RelWithDebInfo )

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

  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	  list(APPEND SoftwareRenderer_Compile_Options_Debug
  		/Od
		/Ob0
        /Oi
		/source-charset:utf-8
		/std:c++latest
		/D_UNICODE
		/GR-
		/U_MBCS
		/W4
		/WX
		/Zi
		/diagnostics:caret
		-Wno-ignored-qualifiers
        -Wno-char-subscripts
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
        -Wno-reorder-ctor
        -Wno-unused-but-set-variable
        -Wno-unused-const-variable
        -Wno-delete-non-abstract-non-virtual-dtor
        -Wno-unused-private-field
        -mavx2
        -mfma
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_MinSizeRel
  		/O1
		/Ob1
		/source-charset:utf-8
		/std:c++latest
		/D_UNICODE
		/GR-
		/U_MBCS
		/W4
		/WX
		/Zi
		/diagnostics:caret
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
  		/O2
		/Ob1
		/source-charset:utf-8
		/std:c++latest
		/D_UNICODE
		/GR-
		/U_MBCS
		/W4
		/WX
		/Zi
		/diagnostics:caret
		-Wno-ignored-qualifiers
        -Wno-char-subscripts
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
        -Wno-reorder-ctor
        -Wno-unused-but-set-variable
        -Wno-unused-const-variable
        -Wno-delete-non-abstract-non-virtual-dtor
        -Wno-unused-private-field
        -mavx2
        -mfma
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_Release
  		/Os
		/Ob2
		/source-charset:utf-8
		/std:c++latest
		/D_UNICODE
		/GR-
		/U_MBCS
		/W4
		/WX
		/sdl-
		/GS-
		/Zi
		/diagnostics:caret
		-flto
		-fvirtual-function-elimination
		-Wno-ignored-qualifiers
        -Wno-char-subscripts
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
        -Wno-reorder-ctor
        -Wno-unused-but-set-variable
        -Wno-unused-const-variable
        -Wno-delete-non-abstract-non-virtual-dtor
        -Wno-unused-private-field
        -mavx2
        -mfma
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Debug
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Link_Options_MinSizeRel
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Release
		/INCREMENTAL:NO
		/LTCG:NOSTATUS
		/WX
		/LTCG
		/OPT:REF
		/DEBUG # Generate a PDB even in Release
	  )
  else()
	  list(APPEND SoftwareRenderer_Compile_Options_Debug
  		/Od
		/Ob0
        /Oi
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

	  list(APPEND SoftwareRenderer_Compile_Options_MinSizeRel
  		/O1
		/Ob1
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

	  list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
  		/O2
		/Ob1
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
  		/Os
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
		/Gw
		/diagnostics:caret
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Debug
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Link_Options_MinSizeRel
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
		/INCREMENTAL:NO
		/WX
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Release
		/INCREMENTAL:NO
		/LTCG:NOSTATUS
		/WX
		/LTCG
		/OPT:REF
		/DEBUG # Generate a PDB even in Release
	  )
  endif()

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

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_MinSizeRel})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_MinSizeRel})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_RelWithDebInfo})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_RelWithDebInfo})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release})

	else()
		message(FATAL_ERROR "Must specify -DCMAKE_BUILD_TYPE=Debug|MinSizeRel|RelWithDebInfo|Release when using Ninja.")
	endif()

	source_group("Headers" FILES ${SoftwareRenderer_Header_Files})
	source_group("Source" FILES ${SoftwareRenderer_Source_Files})

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
