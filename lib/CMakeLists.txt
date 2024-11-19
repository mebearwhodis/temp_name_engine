# lib/CMakeLists.txt

# Collect source files from all subfolders
file(GLOB_RECURSE COMMON_SRC_FILES common/src/*.cc)
file(GLOB_RECURSE MATH_SRC_FILES math/src/*.cc)
file(GLOB_RECURSE PHYSICS_SRC_FILES physics/src/*.cc)

# Collect header files from all subfolders
file(GLOB_RECURSE COMMON_HEADER_FILES common/include/*.h)
file(GLOB_RECURSE MATH_HEADER_FILES math/include/*.h)
file(GLOB_RECURSE PHYSICS_HEADER_FILES physics/include/*.h)

file(GLOB_RECURSE TEST_FILES test/*.cc)

# Create the lib library
add_library(lib STATIC ${COMMON_SRC_FILES} ${MATH_SRC_FILES} ${PHYSICS_SRC_FILES} ${COMMON_HEADER_FILES} ${MATH_HEADER_FILES} ${PHYSICS_HEADER_FILES})

# Specify include directories for lib
target_include_directories(lib PUBLIC common/include math/include physics/include)

# Link libraries to lib (if needed)
# target_link_libraries(lib PUBLIC SDL2::SDL2 imgui::imgui) # Uncomment if necessary

# Link Tracy if enabled
if(ENABLE_PROFILING)
	target_link_libraries(lib PUBLIC Tracy::TracyClient)
	target_compile_definitions(lib PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)

# Enable warnings as errors (W3/WX options)
if(MSVC)
	target_compile_options(lib PRIVATE /W3 /WX)
else()
	target_compile_options(lib PRIVATE -Wall -Wextra -Werror)
endif()

# Create the test executable
add_executable(core_test ${TEST_FILES})

# Link GTest and lib to the test executable
target_link_libraries(core_test PRIVATE lib GTest::gtest GTest::gtest_main)

# Add the test to the CTest system
add_test(Core core_test)
