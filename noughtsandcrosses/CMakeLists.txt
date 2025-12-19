project(noughtsandcrosses)

file(GLOB_RECURSE NOUGHTSANDCROSSES_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

message(STATUS "Files: ${NOUGHTSANDCROSSES_SRC_FILES}")

add_executable(noughtsandcrosses ${NOUGHTSANDCROSSES_SRC_FILES} ${UTILS_FILES})
target_link_libraries(noughtsandcrosses raylib)
target_link_libraries(noughtsandcrosses SAP)

set_target_properties(noughtsandcrosses PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(noughtsandcrosses PROPERTIES LINK_SEARCH_END_STATIC ON)