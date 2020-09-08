add_library(ZSharp)

set(ASSETS
	db_schema.json
	pyramid.json
	pyramids.json
	triangles.json
	ZSharpInit.ini
)

set(ASSET_PATHS
	${CMAKE_SOURCE_DIR}/ZSharp/db_schema.json
	${CMAKE_SOURCE_DIR}/ZSharp/pyramid.json
	${CMAKE_SOURCE_DIR}/ZSharp/pyramids.json
	${CMAKE_SOURCE_DIR}/ZSharp/triangles.json
	${CMAKE_SOURCE_DIR}/ZSharp/ZSharpInit.ini
)

set_property(
	TARGET ZSharp
	PROPERTY ZSHARP_ASSETS ${ASSET_PATHS}
)

target_sources(ZSharp
    PRIVATE
        AssetLoader.h
        Camera.h
        Common.h
        Constants.h
        FileUtilities.h
		Framebuffer.h
        GlobalEdgeTable.h
        IndexBuffer.h
        IntelIntrinsics.h
        JsonObject.h
		JsonScanner.h
        Mesh.h
        Model.h
        Renderer.h
        Scanner.h
		Token.h
        Triangle.h
        UtilMath.h
        VertexBuffer.h
        ZAlgorithm.h
        ZColor.h
        ZConfig.h
        ZDrawing.h
        ZMatrix.h
        ZVector.h
		
        AssetLoader.cpp
        FileUtilities.cpp
		Framebuffer.cpp
        GlobalEdgeTable.cpp
        IndexBuffer.cpp
        IntelIntrinsics.cpp
        JsonObject.cpp
		JsonScanner.cpp
        Renderer.cpp
        Scanner.cpp
		Token.cpp
        UtilMath.cpp
        ZColor.cpp
        ZConfig.cpp
        ZDrawing.cpp
		
		${JSON_ASSETS}
)

set(ZSharp_Compile_Options 
	/W4
	/WX
	/sdl-
	/GR-
	/GS-
	/arch:AVX512
	/std:c++17
	/MP
)

target_compile_options(ZSharp PRIVATE 
	${ZSharp_Compile_Options}
)
