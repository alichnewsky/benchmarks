//
// Created by anthony on 8/26/18.
//
#include "memcopy.h"
#include <cstring>

void memcopy( void* dst, void* src, std::size_t nBytes){

    std::memcpy( dst,  src, nBytes);

}

// multiple dispatch for multiple opt levels?
// multiple implementations