cmake_minimum_required(VERSION 3.12)
project(Snake)

if(UNIX)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wdocumentation -pthread")
    if(CMAKE_GENERATOR STREQUAL "Ninja")
      set(CMAKE_CXX_FLAGS "-fcolor-diagnostics ${CMAKE_CXX_FLAGS}")
    endif( CMAKE_GENERATOR STREQUAL "Ninja")
  endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti -fno-exceptions -std=c++14" CACHE string "Common C++ compiler flags" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE string "C++ compiler flags (Release)" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O1 -g -DDEBUG" CACHE string "C++ compiler flags (Debug)" FORCE)
endif(UNIX)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SNAKE_SOURCES ${CMAKE_SOURCE_DIR}/Snake.cpp)

add_executable(Snake ${SNAKE_SOURCES})

add_subdirectory(test)
