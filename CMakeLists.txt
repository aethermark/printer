cmake_minimum_required(VERSION 3.31)

project(
    Printer
    VERSION 0.1.0
    DESCRIPTION "A debug printer for c++."
    LANGUAGES CXX
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(PRINTER_IS_TOP_LEVEL ON)
else()
    set(PRINTER_IS_TOP_LEVEL OFF)
endif()

if(PRINTER_IS_TOP_LEVEL)
    include(CTest)
else()
    set(BUILD_TESTING OFF)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Options
# ============================================================================

option(PRINTER_BUILD_PLAYGROUND "Build playground" ${PRINTER_IS_TOP_LEVEL})
option(PRINTER_BUILD_BENCHMARKS "Build benchmarks" ${PRINTER_IS_TOP_LEVEL})
option(PRINTER_ENABLE_COVERAGE "Enable coverage instrumentation" ${PRINTER_IS_TOP_LEVEL})
option(PRINTER_BUILD_STATIC_ANALYSIS "Build the static analysis translation unit" ${PRINTER_IS_TOP_LEVEL})

# ============================================================================
# Library
# ============================================================================

add_library(printer STATIC)

add_library(printer::printer ALIAS printer)

file(GLOB_RECURSE PRINTER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(printer
    PRIVATE
    ${PRINTER_SOURCES}
)

target_include_directories(printer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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
# Static analysis translation unit
# ============================================================================

if(PRINTER_BUILD_STATIC_ANALYSIS)

    file(
        GLOB_RECURSE PRINTER_PUBLIC_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    )

    set(HEADER_INCLUDES "")

    foreach(header ${PRINTER_PUBLIC_HEADERS})
        file(
            RELATIVE_PATH rel
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${header}"
        )

        string(APPEND HEADER_INCLUDES "#include <${rel}>\n")
    endforeach()

    configure_file(
        cmake/static_analysis.cpp.in
        "${CMAKE_CURRENT_BINARY_DIR}/static_analysis.cpp"
        @ONLY
    )

    add_library(printer-static-analysis OBJECT
        "${CMAKE_CURRENT_BINARY_DIR}/static_analysis.cpp"
    )

    target_link_libraries(printer-static-analysis
        PRIVATE printer
    )

endif()

# ============================================================================
# Coverage
# ============================================================================

if(PRINTER_IS_TOP_LEVEL AND PRINTER_ENABLE_COVERAGE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(EnableCoverage)
  enable_coverage(printer)
endif()

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
