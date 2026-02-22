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

set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "" FORCE)

set(SoftwareRenderer_Compile_Options_Debug )

set(SoftwareRenderer_Compile_Options_MinSizeRel )

set(SoftwareRenderer_Compile_Options_RelWithDebInfo )

set(SoftwareRenderer_Compile_Options_Release )

set(SoftwareRenderer_Link_Options_Debug )

set(SoftwareRenderer_Link_Options_MinSizeRel )

set(SoftwareRenderer_Link_Options_RelWithDebInfo )

set(SoftwareRenderer_Link_Options_Release )

set(SoftwareRenderer_Preprocessor_Defines_Debug HW_PLATFORM_X86)

set(SoftwareRenderer_Preprocessor_Defines_Release HW_PLATFORM_X86)

if(WIN32)
  set(SoftwareRenderer_Preprocessor_Defines_Common 
    PLATFORM_WINDOWS
    _CRT_SECURE_NO_WARNINGS
	_HAS_EXCEPTIONS=0
    WIN32_LEAN_AND_MEAN
    PLATFORM_MAX_PATH=260
    BUILD_TYPE="${CMAKE_BUILD_TYPE}"
  )

  list(APPEND SoftwareRenderer_Preprocessor_Defines_Debug
    _DEBUG
    ${SoftwareRenderer_Preprocessor_Defines_Common}
  )

  list(APPEND SoftwareRenderer_Preprocessor_Defines_Release 
    NDEBUG
    ${SoftwareRenderer_Preprocessor_Defines_Common}
  )

  if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM"))
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
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
		-Wno-int-to-pointer-cast
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
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
		-Wno-int-to-pointer-cast
        -mavx2
        -mfma
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
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
        -Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
		-Wno-int-to-pointer-cast
        -mavx2
        -mfma
	  )

	  list(APPEND SoftwareRenderer_Compile_Options_Release
        /O1
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
		/Gw
		/Gy
		-Wno-sign-compare
        -Wno-deprecated-copy-with-user-provided-copy
		-Wno-int-to-pointer-cast
        -mavx2
        -mfma
	  )

	  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	    list(APPEND SoftwareRenderer_Compile_Options_Release
		  -flto
          -fvirtual-function-elimination
	    )

		list(APPEND SoftwareRenderer_Link_Options_Release
		    /LTCG
		    /LTCG:NOSTATUS
			/OPT:REF
		)
	  endif()

	  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM")
		  # NOTE: ICX fails to generate a PDB unless the lowercase variant of "/debug" is set.
		  list(APPEND SoftwareRenderer_Link_Options_Debug
			/debug
		  )

		  list(APPEND SoftwareRenderer_Link_Options_MinSizeRel
			/debug
		  )

		  list(APPEND SoftwareRenderer_Link_Options_RelWithDebInfo
			/debug
		  )

		  list(APPEND SoftwareRenderer_Link_Options_Release
			/debug
		  )
	  endif()

	  list(APPEND SoftwareRenderer_Link_Options_Debug
		/INCREMENTAL:NO
		/WX
		/DEBUG
	  )

	  list(APPEND SoftwareRenderer_Link_Options_MinSizeRel
		/INCREMENTAL:NO
		/WX
		/DEBUG
	  )

	  list(APPEND SoftwareRenderer_Link_Options_RelWithDebInfo
		/INCREMENTAL:NO
		/WX
		/DEBUG
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Release
		/INCREMENTAL:NO
		/WX
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

	  # VS2026
	  if(${MSVC_VERSION} GREATER_EQUAL 1950)
		list(APPEND SoftwareRenderer_Compile_Options_MinSizeRel
			/jumptablerdata
		)

		list(APPEND SoftwareRenderer_Compile_Options_RelWithDebInfo
			/jumptablerdata
		)

		list(APPEND SoftwareRenderer_Compile_Options_Release
			/jumptablerdata
		)
	  endif()

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

	  list(APPEND SoftwareRenderer_Compile_Options_Release
  		/O1
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
		/Gy
		/diagnostics:caret
	  )

	  list(APPEND SoftwareRenderer_Link_Options_Debug
		/INCREMENTAL:NO
		/WX
		/DEBUG
	  )

	  list(APPEND SoftwareRenderer_Link_Options_MinSizeRel
		/INCREMENTAL:NO
		/WX
		/DEBUG
	  )

	  list(APPEND SoftwareRenderer_Link_Options_RelWithDebInfo
		/INCREMENTAL:NO
		/WX
		/DEBUG
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

endif()

if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Debug})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Debug})

		target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Debug})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_MinSizeRel})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_MinSizeRel})

		target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Debug})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_RelWithDebInfo})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_RelWithDebInfo})

		target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Debug})

	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release})

		target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Release})

	elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release_PGO_Collect"))
        set(SoftwareRenderer_Compile_Options_Release_PGO_Collect
            ${SoftwareRenderer_Compile_Options_Release}
			-gdwarf
			-gline-tables-only
			/clang:-fdebug-info-for-profiling
			/clang:-funique-internal-linkage-names
        )

		set(SoftwareRenderer_Link_Options_Release_PGO_Collect
			/INCREMENTAL:NO
			/LTCG:NOSTATUS
			/WX
			/LTCG
			/OPT:REF
			/DEBUG:dwarf # Generate a PDB even in Release
        )

		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release_PGO_Collect})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release_PGO_Collect})

        target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Release})

    elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release_PGO_Optimize"))
        set(SoftwareRenderer_Compile_Options_Release_PGO_Optimize
            ${SoftwareRenderer_Compile_Options_Release}
			-fprofile-sample-use=${CMAKE_BINARY_DIR}/SoftwareRenderer/profile.prof
        )

		set(SoftwareRenderer_Link_Options_Release_PGO_Optimize
			${SoftwareRenderer_Link_Options_Release}
        )

		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release_PGO_Optimize})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release_PGO_Optimize})

        target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Release})

	elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM") AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release_PGO_Collect"))
        set(SoftwareRenderer_Compile_Options_Release_PGO_Collect
            ${SoftwareRenderer_Compile_Options_Release}
        )

		set(SoftwareRenderer_Link_Options_Release_PGO_Collect
			/INCREMENTAL:NO
			/WX
			/DEBUG # Generate a PDB even in Release
        )

		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release_PGO_Collect})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release_PGO_Collect})

        target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Release})

    elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM") AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Release_PGO_Optimize"))
        set(SoftwareRenderer_Compile_Options_Release_PGO_Optimize
            ${SoftwareRenderer_Compile_Options_Release}
        )

		set(SoftwareRenderer_Link_Options_Release_PGO_Optimize
			/INCREMENTAL:NO
			/WX
			/DEBUG # Generate a PDB even in Release
        )

		target_compile_options(SoftwareRenderer PRIVATE 
			${SoftwareRenderer_Compile_Options_Release_PGO_Optimize})

		target_link_options(SoftwareRenderer PUBLIC 
			${SoftwareRenderer_Link_Options_Release_PGO_Optimize})

        target_compile_definitions(SoftwareRenderer PRIVATE 
            ${SoftwareRenderer_Preprocessor_Defines_Release})

	else()
		message(FATAL_ERROR "Must specify -DCMAKE_BUILD_TYPE=Debug|MinSizeRel|RelWithDebInfo|Release when using Ninja.")
	endif()

	source_group("Headers" FILES ${SoftwareRenderer_Header_Files})
	source_group("Source" FILES ${SoftwareRenderer_Source_Files})

else()
	target_compile_options(SoftwareRenderer PRIVATE 
		"$<$<CONFIG:DEBUG>:${SoftwareRenderer_Compile_Options_Debug}>")

	target_link_options(SoftwareRenderer PUBLIC 
		"$<$<CONFIG:DEBUG>:${SoftwareRenderer_Link_Options_Debug}>")

	target_compile_definitions(SoftwareRenderer PRIVATE 
        "$<$<CONFIG:DEBUG>:${SoftwareRenderer_Preprocessor_Defines_Debug}>")

	target_compile_options(SoftwareRenderer PRIVATE 
		"$<$<CONFIG:MINSIZEREL>:${SoftwareRenderer_Compile_Options_MinSizeRel}>")

	target_link_options(SoftwareRenderer PUBLIC 
		"$<$<CONFIG:MINSIZEREL>:${SoftwareRenderer_Link_Options_MinSizeRel}>")

	target_compile_definitions(SoftwareRenderer PRIVATE 
        "$<$<CONFIG:MINSIZEREL>:${SoftwareRenderer_Preprocessor_Defines_Debug}>")

	target_compile_options(SoftwareRenderer PRIVATE 
		"$<$<CONFIG:RELWITHDEBINFO>:${SoftwareRenderer_Compile_Options_RelWithDebInfo}>")

	target_link_options(SoftwareRenderer PUBLIC 
		"$<$<CONFIG:RELWITHDEBINFO>:${SoftwareRenderer_Link_Options_RelWithDebInfo}>")

	target_compile_definitions(SoftwareRenderer PRIVATE 
        "$<$<CONFIG:RELWITHDEBINFO>:${SoftwareRenderer_Preprocessor_Defines_Debug}>")

	target_compile_options(SoftwareRenderer PRIVATE 
		"$<$<CONFIG:RELEASE>:${SoftwareRenderer_Compile_Options_Release}>")

	target_link_options(SoftwareRenderer PUBLIC 
		"$<$<CONFIG:RELEASE>:${SoftwareRenderer_Link_Options_Release}>")

	target_compile_definitions(SoftwareRenderer PRIVATE 
        "$<$<CONFIG:RELEASE>:${SoftwareRenderer_Preprocessor_Defines_Release}>")
endif()
