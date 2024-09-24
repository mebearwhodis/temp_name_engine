#api

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(api ${SRC_FILES} ${HEADER_FILES})
target_include_directories(api PUBLIC include/)

target_link_libraries(api PUBLIC core)

if(ENABLE_PROFILING)
	target_link_libraries(api PUBLIC Tracy::TracyClient)
	target_compile_definitions(api PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)