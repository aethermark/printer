add_executable(printer-playground
	main.cpp
)

target_link_libraries(printer-playground
	PRIVATE
	  printer::printer
)

target_include_directories(printer-playground
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

