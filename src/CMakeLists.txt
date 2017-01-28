
file(GLOB STRANSIT_SRC_FILES *.cpp)

include_directories(../libs)

set(STRANSIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(STRANSIT_INCLUDE_DIR ${STRANSIT_INCLUDE_DIR} PARENT_SCOPE)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(TARGET stransit_lib)
  message("already defined stransit_lib")
else()
  message(${STRANSIT_INCLUDE_DIR})
  
  add_library(stransit_lib STATIC ${STRANSIT_SRC_FILES})
endif()
