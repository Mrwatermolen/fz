cmake_minimum_required(VERSION 3.20)

project(FZ VERSION 0.0.0 LANGUAGES CXX)

set(FZ_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(FZ_MAIN_PROJECT ON)
endif()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FZ_PUBLIC_INCLUDE_DIR include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# add header-only library
add_library(fz INTERFACE)
target_include_directories(fz INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${FZ_PUBLIC_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${FZ_PUBLIC_INCLUDE_DIR}>
)

add_subdirectory(test)

set(FZ_CMAKE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake)

install(TARGETS fz EXPORT fzTargets)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fzConfig.cmake.in
  ${FZ_CMAKE_DIR}/fzConfig.cmake
  INSTALL_DESTINATION ${FZ_CMAKE_DIR}
)

export(EXPORT fzTargets
  FILE ${FZ_CMAKE_DIR}/fzTargets.cmake
  NAMESPACE fz::
)
