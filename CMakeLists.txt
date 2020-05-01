cmake_minimum_required( VERSION 3.16 )

##
## PROJECT
##
project(
    NamedType
    VERSION 1.1.1
    LANGUAGES CXX
)

option(
    ${PROJECT_NAME}_BuildTests
    "Build the unit tests when BUILD_TESTING is enabled."
    ON
)
option(
    ${PROJECT_NAME}_Install
    "Install CMake targets during install step."
    ON
)

##
## TARGET: NamedType library
##
add_library( ${PROJECT_NAME} INTERFACE )
add_library( ${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

##
## TARGET headeronlylibsrc for IDE
##
add_custom_target(
    ${PROJECT_NAME}_src
    SOURCES
    "include/NamedType/crtp.hpp"
    "include/NamedType/named_type.hpp"
    "include/NamedType/named_type_impl.hpp"
    "include/NamedType/underlying_functionalities.hpp"
)

##
## TESTS
##
include( CTest )
if( BUILD_TESTING AND ${PROJECT_NAME}_BuildTests )
    enable_testing()
    add_subdirectory( test )
endif()

##
## INSTALL
##
include( GNUInstallDirs )
include( CMakePackageConfigHelpers )
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

if( ${PROJECT_NAME}_Install )
    install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
    export(
        TARGETS ${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
    )
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    install(
        EXPORT "${PROJECT_NAME}Targets"
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
endif()
