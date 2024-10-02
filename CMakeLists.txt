cmake_minimum_required(VERSION 3.18)
project(GameEngine)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(SDL2 CONFIG REQUIRED)

enable_testing()
option(ENABLE_PROFILING "Enable Tracy Profiling" OFF)
find_package(GTest CONFIG REQUIRED)

# Add ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SOURCES
	${IMGUI_DIR}/*.cpp)

add_subdirectory(core/)
add_subdirectory(api/)
add_subdirectory(imgui/)

add_executable(main main.cc ${IMGUI_SOURCES})

target_link_libraries(main PRIVATE core api imgui SDL2::SDL2 SDL2::SDL2main)

target_include_directories(main PRIVATE ${IMGUI_DIR})


if(ENABLE_PROFILING)
	find_package(Tracy CONFIG REQUIRED)
	target_link_libraries(main PUBLIC Tracy::TracyClient)
	target_compile_definitions(main PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)