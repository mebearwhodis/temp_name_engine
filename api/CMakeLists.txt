#api

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(api ${SRC_FILES} ${HEADER_FILES}
		src/shape.cc
		include/shape.h
		src/circle.cc
		include/circle.h
		src/rectangle.cc
		include/rectangle.h)
target_include_directories(api PUBLIC include/)

target_link_libraries(api PUBLIC core SDL2::SDL2 SDL2::SDL2main)


if(ENABLE_PROFILING)
	target_link_libraries(api PUBLIC Tracy::TracyClient)
	target_compile_definitions(api PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)