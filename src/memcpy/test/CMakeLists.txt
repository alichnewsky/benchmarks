
add_executable( test_memcopy test_memcopy.cpp )

target_compile_options( test_memcopy PRIVATE -O3 )

target_link_libraries( test_memcopy
        array_benchmarks
        benchmark_main
        )
        
add_executable( test_basic  test_basic.cpp )

target_link_libraries( test_basic benchmark_main )
        