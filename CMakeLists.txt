cmake_minimum_required(VERSION 3.20)

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

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# add headers and sources
set(am_headers am.h halfint.h racah_reduction.h rme.h wigner_gsl.h
               wigner_gsl_twice.h
)
set(am_sources am.cpp halfint.cpp)

# define am library and add source files
add_library(am ${am_sources})
add_library(am::am ALIAS am)

# ##############################################################################
# find external projects/dependencies
# ##############################################################################

if(NOT TARGET fmt::fmt)
  find_package(fmt)
endif()

if(TARGET fmt::fmt)
  list(APPEND am_headers halfint_fmt.h)
  target_link_libraries(am fmt::fmt)
  message(STATUS "building am with fmt support")
endif()

# ##############################################################################
# define include directory
# ##############################################################################
cmake_path(GET am_SOURCE_DIR PARENT_PATH am_PARENT_DIR)
target_include_directories(
  am INTERFACE $<BUILD_INTERFACE:${am_PARENT_DIR}> $<INSTALL_INTERFACE:include>
)

# ##############################################################################
# define installation rules
# ##############################################################################
install(TARGETS am EXPORT amTargets)
# if(TARGET fmt::fmt) install(TARGETS fmt::fmt EXPORT amTargets ) endif()
install(FILES ${am_headers} DESTINATION include/am)

install(
  EXPORT amTargets
  NAMESPACE am::
  FILE amTargets.cmake
  DESTINATION lib/cmake/am
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/amConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/am"
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
        DESTINATION lib/cmake/am
)

export(
  EXPORT amTargets
  NAMESPACE am::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/amTargets.cmake"
)
