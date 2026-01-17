project(nacbot_v2)

file(GLOB_RECURSE NACBOT_V2_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

message(STATUS "Files: ${NACBOT_V2_SRC_FILES}")

add_executable(nacbot_v2 ${NACBOT_V2_SRC_FILES} ${UTILS_FILES})
target_link_libraries(nacbot_v2 raylib)
target_link_directories(nacbot_v2 PRIVATE ${CMAKE_SOURCE_DIR}/utils)

set_target_properties(nacbot_v2 PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(nacbot_v2 PROPERTIES LINK_SEARCH_END_STATIC ON)