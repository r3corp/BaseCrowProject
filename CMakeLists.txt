cmake_minimum_required(VERSION 3.20)
project(CppCrudWebApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)
include(cmake/FetchCrow.cmake)
#include(cmake/FetchMustache.cmake) Crow already includes mustache

find_package(SQLite3 REQUIRED)

include_directories(src
  src/rest
  src/services
  src/db
  src/interfaces
  src/models
  src/ui
  src/utils)

file(GLOB_RECURSE SOURCES
  src/*.cpp
  src/rest/*.cpp
  src/services/*.cpp
  src/db/*.cpp
  src/interfaces/*.cpp
  src/models/*.cpp
  src/ui/*.cpp
  src/utils/*.cpp)

add_executable(CppCrudWebApp ${SOURCES})
target_link_libraries(CppCrudWebApp PRIVATE Crow::Crow SQLite::SQLite3)