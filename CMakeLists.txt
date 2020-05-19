cmake_minimum_required(VERSION 3.8)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# change the HINTS path to path of your gcc 9 library
find_library(GCC9
  NAMES libstdc++fs.a
  HINTS "/usr/lib/gcc/x86_64-linux-gnu/9"
)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)

# include gcc9 library
if( NOT GCC9 )
  target_link_libraries(monitor ${CURSES_LIBRARIES})
else()
  target_link_libraries(monitor ${CURSES_LIBRARIES} ${GCC9})
endif()


# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
  message(FATAL_ERROR "Insufficient gcc version. Version 9 required.")
endif()
