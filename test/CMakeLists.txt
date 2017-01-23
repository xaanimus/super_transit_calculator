
add_subdirectory(../src shared_lib)

file(GLOB TEST_SRC_FILES *.cpp)

include_directories(${STRANSIT_INCLUDE_DIR})
include_directories(../libs)

add_executable(stransit_test ${TEST_SRC_FILES})
target_link_libraries(stransit_test stransit_lib)

