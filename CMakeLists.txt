# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.18)
project(GameEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find required packages
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Option for profiling
option(ENABLE_PROFILING "Enable Tracy Profiling" OFF)

# Add subdirectories for api and lib
add_subdirectory(api)
add_subdirectory(lib)

# Create the main executable
add_executable(main main.cc)

# Link libraries to main
target_link_libraries(main PUBLIC api lib SDL2::SDL2 SDL2::SDL2main imgui::imgui)

# Enable Tracy profiling if the option is set
if(ENABLE_PROFILING)
	find_package(Tracy CONFIG REQUIRED)
	target_link_libraries(main PUBLIC Tracy::TracyClient)
	target_compile_definitions(main PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)

# Enable warnings as errors (W3 and WX options)
if(MSVC)
	# For MSVC compilers, use /W3 and /WX for warnings and errors
	target_compile_options(main PRIVATE /W3 /WX)
	target_compile_options(api PRIVATE /W3 /WX)
	target_compile_options(lib PRIVATE /W3 /WX)
else()
	# For non-MSVC compilers, enable flags for GCC/Clang
	target_compile_options(main PRIVATE -Wall -Wextra -Werror)
	target_compile_options(api PRIVATE -Wall -Wextra -Werror)
	target_compile_options(lib PRIVATE -Wall -Wextra -Werror)
endif()
