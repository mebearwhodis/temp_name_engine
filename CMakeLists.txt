cmake_minimum_required(VERSION 3.18)
project(GameEngine)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

enable_testing()
option(ENABLE_PROFILING "Enable Tracy Profiling" OFF)
    
find_package(GTest CONFIG REQUIRED)

if(ENABLE_PROFILING)
 	find_package(Tracy CONFIG REQUIRED)
endif(ENABLE_PROFILING)

add_subdirectory(core/)
add_subdirectory(api/)

add_executable(main main.cc)
target_link_libraries(main PRIVATE core api SDL2::SDL2 SDL2::SDL2main)



if(ENABLE_PROFILING)
	target_link_libraries(main PUBLIC Tracy::TracyClient)
	target_compile_definitions(main PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)