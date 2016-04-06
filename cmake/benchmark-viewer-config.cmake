set(PROJECT_NAME benchmark-viewer)

# If ${SOURCE_DIR} is a git repository VERSION is set to
# `git describe --tags` later.
set(VERSION devel)

set(CODENAME "")

#project(${PROJECT_NAME} C)

# {{{ CFLAGS
if (CMAKE_BUILD_TYPE MATCHES "Release")
  message(STATUS "Compiling in Release mode")
  add_definitions(-O3 -march=native)
elseif( CMAKE_COMPILER_IS_GNUCXX )
  message(STATUS "Compiling in Debug mode with GCC")
  # Add additional GCC options.
  add_definitions(
    -g -O -Wall -pedantic
    -pedantic-errors -Wextra -Wcast-align
    -Wcast-qual -Wchar-subscripts -Wcomment
    -Wdisabled-optimization
    -Werror -Wformat -Wformat=2
    -Wformat-nonliteral -Wformat-security
    -Wformat-y2k
    -Wimport  -Winit-self -Winline
    -Winvalid-pch
    -Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces
    -Wmissing-field-initializers -Wmissing-format-attribute
    -Wmissing-include-dirs -Wmissing-noreturn
    -Wpacked  -Wparentheses  -Wpointer-arith
    -Wredundant-decls -Wreturn-type
    -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch
    -Wswitch-enum -Wtrigraphs  -Wuninitialized
    -Wunknown-pragmas -Wunreachable-code -Wunused
    -Wunused-function -Wunused-label -Wunused-parameter
    -Wunused-value -Wunused-variable -Wvariadic-macros
    -Wvolatile-register-var  -Wwrite-strings
    )
elseif( CMAKE_CXX_COMPILER MATCHES "clang" )
  message(STATUS "Compiling in Debug mode with Clang")
  add_definitions( -Wall -Wextra )
endif()

if(NOT WIN32)
  add_definitions( -fvisibility=hidden )
endif()
# }}}

# {{{ Find external utilities
macro(a_find_program var prg req)
  set(required ${req})
  find_program(${var} ${prg})
  if(NOT ${var})
    message(STATUS "${prg} not found.")
    if(required)
      message(FATAL_ERROR "${prg} is required to build ${PROJECT_NAME}")
    endif()
  else()
    message(STATUS "${prg} -> ${${var}}")
  endif()
endmacro()

a_find_program(GIT_EXECUTABLE git FALSE)
a_find_program(HOSTNAME_EXECUTABLE hostname FALSE)
# }}}

# {{{ Version stamp
if(EXISTS ${SOURCE_DIR}/.git/HEAD AND GIT_EXECUTABLE)
  # get current version
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags
    WORKING_DIRECTORY ${SOURCE_DIR}
    OUTPUT_VARIABLE VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(BUILD_FROM_GIT TRUE)
endif()
# }}}

# {{{ Get hostname
execute_process(
  COMMAND ${HOSTNAME_EXECUTABLE}
  WORKING_DIRECTORY ${SOURCE_DIR}
  OUTPUT_VARIABLE BUILDHOSTNAME
  OUTPUT_STRIP_TRAILING_WHITESPACE)
# }}}

# {{{ Install path and configuration variables
if(DEFINED PREFIX)
  set(PREFIX ${PREFIX} CACHE PATH "install prefix")
  set(CMAKE_INSTALL_PREFIX ${PREFIX})
else()
  set(PREFIX ${CMAKE_INSTALL_PREFIX} CACHE PATH "install prefix")
endif()

# Hide to avoid confusion
mark_as_advanced(CMAKE_INSTALL_PREFIX)

set(BENCHMARK_VIEWER_APPLICATION_NAME ${PROJECT_NAME})
set(BENCHMARK_VIEWER_VERSION          ${VERSION})
set(BENCHMARK_VIEWER_COMPILE_MACHINE  ${CMAKE_SYSTEM_PROCESSOR})
set(BENCHMARK_VIEWER_COMPILE_HOSTNAME ${BUILDHOSTNAME})
set(BENCHMARK_VIEWER_COMPILE_BY       $ENV{USER})
set(BENCHMARK_VIEWER_RELEASE          ${CODENAME})
set(BENCHMARK_VIEWER_DATA_PATH        ${PREFIX}/share/${BENCHMARK_VIEWER_APPLICATION_NAME})
# }}}

# {{{ Configure files
set(BENCHMARK_VIEWER_CONFIGURE_FILES
  config.hh.in
  )

macro(a_configure_file file)
  string(REGEX REPLACE ".in\$" "" outfile ${file})
  message(STATUS "Configuring ${outfile}")
  configure_file(${SOURCE_DIR}/${file}
    ${BUILD_DIR}/${outfile}
    @ONLY)
endmacro()

foreach(file ${BENCHMARK_VIEWER_CONFIGURE_FILES})
  a_configure_file(${file})
endforeach()
#}}}
