set_source_files_properties(memcpy_avx.c PROPERTIES COMPILE_FLAGS -mavx2 )
set_source_files_properties(memcpy_sse41.c PROPERTIES COMPILE_FLAGS -msse4.1)


add_library( array_benchmarks memcopy.cpp memcpy_avx.c memcpy_sse.c memcpy_sse41.c memcopy.h)

target_compile_options( array_benchmarks PRIVATE -O3 )

add_subdirectory(test)