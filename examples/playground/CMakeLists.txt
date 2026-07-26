add_executable(playground
	main.cpp
)

target_link_libraries(playground
	PRIVATE
	  printer::printer
)

target_include_directories(playground
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

