cmake_minimum_required(VERSION 3.18)
project(GameEngine)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(SDL2 CONFIG REQUIRED)
find_package(imgui)


enable_testing()
option(ENABLE_PROFILING "Enable Tracy Profiling" OFF)
find_package(GTest CONFIG REQUIRED)



add_subdirectory(libs/)
add_subdirectory(api/)

add_executable(main main.cc)

target_link_libraries(main PUBLIC  core api SDL2::SDL2 SDL2::SDL2main imgui::imgui)



if(ENABLE_PROFILING)
	find_package(Tracy CONFIG REQUIRED)
	target_link_libraries(main PUBLIC Tracy::TracyClient)
	target_compile_definitions(main PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)