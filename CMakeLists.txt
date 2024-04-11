cmake_minimum_required(VERSION 3.23)

# Determine if am is built as a subproject (using add_subdirectory) or if it is
# the master project.
if(NOT DEFINED AM_MASTER_PROJECT)
  set(AM_MASTER_PROJECT OFF)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(AM_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif()
endif()

project(am CXX)

if(AM_MASTER_PROJECT AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# optionally enable exceptions
option(AM_ENABLE_EXCEPTIONS "enable throwing exceptions for invalid inputs" OFF)

# ##############################################################################
# find external projects/dependencies
# ##############################################################################

if(NOT TARGET fmt::fmt)
  find_package(fmt)
endif()

if(NOT TARGET GSL::gsl)
  find_package(GSL REQUIRED)
endif()

# ##############################################################################
# define am headers and sources
# ##############################################################################

# define units
set(am_UNITS_H halfint wigner_gsl wigner_gsl_twice racah_reduction rme am)
if(TARGET fmt::fmt)
  list(APPEND am_UNITS_H halfint_fmt)
  message(STATUS "building am with fmt support")
endif()

# construct lists of headers and sources
set(am_HEADERS ${am_UNITS_H})
list(TRANSFORM am_HEADERS APPEND .h)

# define am library and add source files
add_library(am INTERFACE)
target_sources(
  am INTERFACE
  FILE_SET HEADERS
  TYPE HEADERS
  FILES ${am_HEADERS}
)
add_library(am::am ALIAS am)

# ##############################################################################
# enable optional features
# ##############################################################################

if(AM_ENABLE_EXCEPTIONS)
  target_compile_definitions(am PUBLIC AM_EXCEPTIONS)
endif()

# ##############################################################################
# link dependencies
# ##############################################################################

target_link_libraries(am INTERFACE m GSL::gsl)
if(TARGET fmt::fmt)
  target_link_libraries(am INTERFACE fmt::fmt)
endif()

# ##############################################################################
# define installation rules
# ##############################################################################
include(GNUInstallDirs)

install(
  TARGETS am
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  EXPORT amTargets
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/am
)

install(
  EXPORT amTargets
  NAMESPACE am::
  FILE amTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/am
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/amConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/am"
  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/amConfigVersion.cmake"
  VERSION "${am_VERSION_MAJOR}.${am_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/amConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/amConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/am
)

export(
  EXPORT amTargets
  NAMESPACE am::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/amTargets.cmake"
)

# ##############################################################################
# define tests
# ##############################################################################

set(am_TEST_NAMES halfint_test am_test)

add_custom_target(am_tests)
foreach(test_name IN LISTS am_TEST_NAMES)
  add_executable(${test_name} EXCLUDE_FROM_ALL ${test_name}.cpp)
  target_link_libraries(${test_name} am::am)
  add_dependencies(am_tests ${test_name})
endforeach()

if(AM_MASTER_PROJECT)
  add_custom_target(tests)
  add_dependencies(tests am_tests)
endif()
