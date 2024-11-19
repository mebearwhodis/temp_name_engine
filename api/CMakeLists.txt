# api/CMakeLists.txt

# Collect all source and header files
file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

# Create the API library
add_library(api ${SRC_FILES} ${HEADER_FILES})

# Include the 'include' directory for the 'api' target
target_include_directories(api PUBLIC include)

# Link lib
target_link_libraries(api PUBLIC lib)

# Link SDL2
target_link_libraries(api PUBLIC SDL2::SDL2 SDL2::SDL2main)

# Link Imgui
target_link_libraries(api PUBLIC imgui::imgui)

# If profiling is enabled, link Tracy
if(ENABLE_PROFILING)
	target_link_libraries(api PUBLIC Tracy::TracyClient)
	target_compile_definitions(api PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)

# Enable warnings as errors (W3/WX options)
if(MSVC)
	target_compile_options(api PRIVATE /W3 /WX)
else()
	target_compile_options(api PRIVATE -Wall -Wextra -Werror)
endif()
