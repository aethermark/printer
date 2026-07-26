cmake_minimum_required(VERSION 3.31)

project(
    Printer
    VERSION 0.1.0
    DESCRIPTION "A debug printer for c++."
    LANGUAGES CXX
)

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Options
# ============================================================================

option(PRINTER_BUILD_PLAYGROUND "Build playground" ON)
option(PRINTER_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(PRINTER_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

# ============================================================================
# Library
# ============================================================================

add_library(printer)

add_library(printer::printer ALIAS printer)

file(GLOB_RECURSE PRINTER_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

target_sources(printer
    PRIVATE
    ${PRINTER_SOURCES}
)

target_include_directories(printer
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(printer
    PUBLIC
        cxx_std_26
)

set_target_properties(printer PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# ============================================================================
# Coverage
# ============================================================================
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnableCoverage)

enable_coverage(printer)

# ============================================================================
# Subdirectories
# ============================================================================

if(PRINTER_BUILD_PLAYGROUND)
    add_subdirectory(examples/playground)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(PRINTER_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# ============================================================================
# Installation
# ============================================================================

install(
    TARGETS printer
    EXPORT PrinterTargets

    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT PrinterTargets
    FILE PrinterTargets.cmake
    NAMESPACE printer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Printer
)

# ============================================================================
# Package Configuration
# ============================================================================

configure_package_config_file(
    cmake/PrinterConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PrinterConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Printer
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PrinterConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/PrinterConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/PrinterConfigVersion.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/Printer
)
