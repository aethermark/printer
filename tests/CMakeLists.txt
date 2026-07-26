include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(printer-tests)

file(GLOB_RECURSE PRINTER_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

target_sources(printer-tests 
    PRIVATE
      ${PRINTER_TEST_SOURCES}
)

target_include_directories(printer-tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnableCoverage)

enable_coverage(printer-tests)

target_link_libraries(printer-tests
    PRIVATE
        printer::printer
        Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(printer-tests)
