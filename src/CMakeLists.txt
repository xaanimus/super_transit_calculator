
file(GLOB STRANSIT_SRC_FILES *.cpp)

set(STRANSIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(STRANSIT_INCLUDE_DIR ${STRANSIT_INCLUDE_DIR} PARENT_SCOPE)

if(TARGET stransit_lib)
  message("already defined stransit_lib")
else()
  message(${STRANSIT_INCLUDE_DIR})
  
  add_library(stransit_lib STATIC ${STRANSIT_SRC_FILES})
endif()
