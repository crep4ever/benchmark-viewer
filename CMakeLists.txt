cmake_minimum_required(VERSION 2.8.11)

project(benchmark-viewer)

# forbid in-source build
if ("${CMAKE_CURRENT_SOURCE_DIR}"  STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR
    "The project ${PROJECT_NAME} requires an out of source build. "
    "Please create a separate binary directory and run CMake there. "
    "For example:\n mkdir build && cd build && cmake ..")
endif()

# sources
set(PROJECT_SOURCES
  src/main.cc
  src/main-window.cc
  src/preferences.cc
  src/parser.cc
  src/node.cc
  src/scene.cc
  src/timeline-view.cc
  src/overlay-widget.cc
  src/graphics-node-item.cc
  src/node-info-widget.cc
  src/info-widget.cc
)

# resources
set(PROJECT_RCS
  icons/benchmark-viewer/benchmark.qrc
  icons/tango/tango.qrc
  benchmark.rc
)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Version information
include(GetGitRevisionDescription)
git_describe(VERSION --tags)

string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message(STATUS "Git version ${VERSION_SHORT}")

# Configure a header file to pass some of the CMake settings to the source code
set(PROJECT_APPLICATION_NAME ${PROJECT_NAME})
set(PROJECT_VERSION          ${VERSION_SHORT})
set(PROJECT_COMPILE_MACHINE  ${CMAKE_SYSTEM_PROCESSOR})
set(PROJECT_COMPILE_HOSTNAME ${BUILDHOSTNAME})
set(PROJECT_COMPILE_BY       $ENV{USER})
set(PROJECT_DATA_PATH        ${PREFIX}/share/${MATRIX_VIEWER_APPLICATION_NAME})

configure_file(
  "${PROJECT_SOURCE_DIR}/config.hh.in"
  "${PROJECT_BINARY_DIR}/config.hh"
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_BINARY_DIR ON)

# Instruct CMake to run moc/rcc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find external dependencies
find_package(Qt5Widgets REQUIRED)

list(APPEND LIBRARIES Qt5::Widgets)

# Compiler flags
set(CMAKE_CXX_STANDARD 11)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(FLAGS -g -Ofast -march=native -DNDEBUG)
  set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed,-rdynamic")
else ()
  set(FLAGS -g -O1 -Werror -Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls)
  set (CMAKE_SHARED_LINKER_FLAGS "--as-needed,-rdynamic")
endif ()


message (STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")
message (STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message (STATUS "Compiler flags: ${FLAGS}")
message (STATUS "Linker flags: ${CMAKE_SHARED_LINKER_FLAGS}")

# Build the main executable
add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} ${PROJECT_RCS})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Installation
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
install(FILES icons/benchmark-viewer/src/benchmark-viewer.svg DESTINATION share/icons)
install(FILES ${PROJECT_PROFILES} DESTINATION ${PROJECT_DATA_PATH}/profiles)
