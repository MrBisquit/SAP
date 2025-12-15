project(dotsandboxes)

file(GLOB_RECURSE DOTSANDBOXES_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(dotsandboxes ${DOTSANDBOXES_SRC_FILES} ${UTILS_FILES})
target_link_libraries(dotsandboxes raylib)

set_target_properties(dotsandboxes PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(dotsandboxes PROPERTIES LINK_SEARCH_END_STATIC ON)