cmake_minimum_required(VERSION 2.8)
set(CMAKE_COLOR_MAKEFILE ON)
#set(CMAKE_VERBOSE_MAKEFILE OFF)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_MODULE_PATH "${SOURCE_DIR}/cmake/")

if(NOT EXISTS ${CMAKE_MODULE_PATH}/benchmark-viewer-config.cmake)
  message(FATAL_ERROR "Cannot find: ${CMAKE_MODULE_PATH}/benchmark-viewer-config.cmake")
endif()

include(${CMAKE_MODULE_PATH}/benchmark-viewer-config.cmake)

# {{{ Sources
set(BENCHMARK_VIEWER_SOURCES
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

# header (moc)
set(BENCHMARK_VIEWER_QT_HEADER
  src/main-window.hh
  src/preferences.hh
  src/scene.hh
  src/timeline-view.hh
  src/overlay-widget.hh
  src/node-info-widget.hh
  src/info-widget.hh
  )

# resources (rcs)
set(BENCHMARK_VIEWER_RCS
  icons/benchmark-viewer/benchmark.qrc
  icons/tango/tango.qrc
  benchmark.rc
  )

# }}}

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

include(${QT_USE_FILE})

include_directories(
  ${SOURCE_DIR}
  ${SOURCE_DIR}/src/
  ${BUILD_DIR}
  )

# process Qt special files
qt4_wrap_cpp(BENCHMARK_VIEWER_MOCS ${BENCHMARK_VIEWER_QT_HEADER})
qt4_add_resources(BENCHMARK_VIEWER_RESSOURCES ${BENCHMARK_VIEWER_RCS})
qt4_wrap_ui(BENCHMARK_VIEWER_UI_HDRS ${BENCHMARK_VIEWER_UIS})

# generating executable
add_executable(${BENCHMARK_VIEWER_APPLICATION_NAME} WIN32
  ${BENCHMARK_VIEWER_SOURCES}
  ${BENCHMARK_VIEWER_MOCS}
  ${BENCHMARK_VIEWER_RESSOURCES}
  ${COMPILED_TRANSLATIONS}
  ${BENCHMARK_VIEWER_UI_HDRS}
  )

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-enable-auto-import")
endif(WIN32)

list(APPEND LIBRARIES ${QT_LIBRARIES})

target_link_libraries(${BENCHMARK_VIEWER_APPLICATION_NAME} ${LIBRARIES})

# {{{ Internationalization
set (TRANSLATIONS
#lang/benchmark-viewer_fr.ts
)

set (COMPILED_TRANSLATIONS
#lang/benchmark-viewer_fr.qm
)

add_custom_target(
  translations_target
  COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
  DEPENDS ${TRANSLATIONS}
  COMMENT "Updating translations"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#uncomment and run `make translations_target` to update translations ts files from sources
#warning, if uncommented, running make clean will remove ts files
#qt4_create_translation(COMPILED_TRANSLATIONS ${BENCHMARK_VIEWER_SOURCES} ${TRANSLATIONS} )
# }}}

# {{{ Installation
install(TARGETS ${BENCHMARK_VIEWER_APPLICATION_NAME} RUNTIME DESTINATION bin)
install(FILES ${BENCHMARK_VIEWER_APPLICATION_NAME}.desktop DESTINATION share/applications)
install(FILES icons/benchmark-viewer/src/benchmark-viewer.svg DESTINATION share/icons)
install(FILES ${COMPILED_TRANSLATIONS} DESTINATION ${BENCHMARK_VIEWER_DATA_PATH}/lang)
# }}}
