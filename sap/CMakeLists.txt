file(GLOB_RECURSE SAP_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

set(RAYLIB_VERBOSE OFF)

add_library(SAP STATIC ${SAP_FILES})
target_link_libraries(SAP raylib)