function(enable_coverage target)
    if(GLYPH_ENABLE_COVERAGE)
        target_compile_options(${target} PRIVATE
            --coverage
            -O0
            -g
        )

        target_link_options(${target} PRIVATE
            --coverage
        )
    endif()
endfunction()
