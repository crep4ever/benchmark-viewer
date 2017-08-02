cmake_minimum_required(VERSION 2.8.11)

project(benchmark-viewer)

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
set(PROJECT_VERSION          ${VERSION})
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
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Release")
  set(FLAGS -Ofast -march=native -DNDEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(FLAGS -g -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif ()

message (STATUS "Using ${CMAKE_CXX_COMPILER} compiler")
message (STATUS "Compile in ${CMAKE_BUILD_TYPE} configuration")
message (STATUS "Compile options: ${FLAGS}")

# Build the main executable
add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} ${PROJECT_RCS})
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Installation
install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
install(FILES icons/benchmark-viewer/256x256/benchmark-viewer.png DESTINATION share/icons)
install(FILES ${PROJECT_PROFILES} DESTINATION ${PROJECT_DATA_PATH}/profiles)
