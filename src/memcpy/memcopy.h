//
// Created by anthony on 8/26/18.
//
#pragma once
#include <cstdint>

#ifdef __cplusplus 
using std::size_t;
extern "C" {
#endif
    void memcopy( void* dst, void* src, std::size_t nBytes);
    // FIXME : write AVX2 , AVX512 variants.
    
    int memcpy_uncached_store_avx(void *dest, const void *src, size_t n_bytes);
    int memcpy_uncached_store_sse(void *dest, const void *src, size_t n_bytes);
    
    int memcpy_cached_store_avx(void *dest, const void *src, size_t n_bytes);
    int memcpy_cached_store_sse(void *dest, const void *src, size_t n_bytes);
    
    int memcpy_uncached_load_sse41(void *dest, const void *src, size_t n_bytes);

#ifdef __cplusplus
}
#endif