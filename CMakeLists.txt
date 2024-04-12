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
# define headers and sources
# ##############################################################################

# define units
set(${PROJECT_NAME}_UNITS_H halfint wigner_gsl wigner_gsl_twice racah_reduction rme am)
if(TARGET fmt::fmt)
  list(APPEND ${PROJECT_NAME}_UNITS_H halfint_fmt)
  message(STATUS "building am with fmt support")
endif()

# construct lists of headers and sources
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_UNITS_H})
list(TRANSFORM ${PROJECT_NAME}_HEADERS APPEND .h)
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH parent_path)

# define library and add source files
add_library(${PROJECT_NAME} INTERFACE)
target_sources(
  ${PROJECT_NAME} INTERFACE
  FILE_SET HEADERS
  TYPE HEADERS
  BASE_DIRS "${parent_dir}"
  FILES ${${PROJECT_NAME}_HEADERS}
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ##############################################################################
# enable optional features
# ##############################################################################

if(AM_ENABLE_EXCEPTIONS)
  target_compile_definitions(${PROJECT_NAME} PUBLIC AM_EXCEPTIONS)
endif()

# ##############################################################################
# link dependencies
# ##############################################################################

target_link_libraries(${PROJECT_NAME} INTERFACE m GSL::gsl)
if(TARGET fmt::fmt)
  target_link_libraries(${PROJECT_NAME} INTERFACE fmt::fmt)
endif()

# ##############################################################################
# define installation rules
# ##############################################################################
include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
)

install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

export(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

# ##############################################################################
# define tests
# ##############################################################################

set(${PROJECT_NAME}_UNITS_TEST halfint_test ${PROJECT_NAME}_test)

add_custom_target(${PROJECT_NAME}_tests)
foreach(test_name IN LISTS ${PROJECT_NAME}_UNITS_TEST)
  add_executable(${test_name} EXCLUDE_FROM_ALL ${test_name}.cpp)
  target_link_libraries(${test_name} ${PROJECT_NAME}::${PROJECT_NAME})
  add_dependencies(${PROJECT_NAME}_tests ${test_name})
endforeach()

if(AM_MASTER_PROJECT)
  add_custom_target(tests)
  add_dependencies(tests ${PROJECT_NAME}_tests)
endif()
