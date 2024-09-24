#core

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE TEST_FILES test/*.cc)

add_library(core ${SRC_FILES} ${HEADER_FILES})
target_include_directories(core PUBLIC include/)

add_executable(core_test ${TEST_FILES})
target_link_libraries(core_test PRIVATE core GTest::gtest GTest::gtest_main)

if(ENABLE_PROFILING)
	target_link_libraries(core PUBLIC Tracy::TracyClient)
	target_compile_definitions(core PUBLIC TRACE_ENABLE=1)
endif(ENABLE_PROFILING)

add_test(Core core_test)