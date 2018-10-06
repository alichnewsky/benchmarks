# Simple benchmarks


demonstrates the use of [google benchmark](http://github.com/google/benchmark) on extermely simple benchmarks.


## clone this repo

```
git clone --recursive https://github.com/alichnewsky/benchmarks
```

includes [Google Benchmark](https://github.com/google/benchmark) and [Google Test](https://google.com/google/googletest) third party dependencies as submodules.

## build
```
cd /where/you/cloned; mkdir build ; cd build && cmake ../ && make -j $(nproc)
```

## Licenses and Copyright

- [OpenMP STREAM benchmark was obtained from NERSC](http://www.nersc.gov/users/computational-systems/cori/nersc-8-procurement/trinity-nersc-8-rfp/nersc-8-trinity-benchmarks/stream/)
   - [Download original code here](http://www.nersc.gov/assets/Trinity--NERSC-8-RFP/Benchmarks/Jan9/stream.tar)
   - Copyright is to Joh McCalpin
   - [original code can be found here](https://www.cs.virginia.edu/stream/FTP/Code/) including the license.
- `cmake/benchmark.cmake` and `cmake/gtest.cmake` are adaptations based on [gRPC](https://github.com/grpc/grpc) cmake files. As such, they are licensed under [Apache v2 license](https://github.com/grpc/grpc/blob/master/LICENSE).
- `src/memcpy/test/test_basic.cc` is from Google Benchmark library, and is therefore also licensed under [Apache v2 license](https://github.com/google/benchmark/blob/master/LICENSE). See said project [AUTHORS](https://github.com/google/benchmark/blob/master/AUTHORS) and [CONTRIBUTORS](https://github.com/google/benchmark/blob/master/CONTRIBUTORS) files for copyright holders.
- `src/memcpy/memcpy_{avx,sse,sse41}.c` are [NVIDIA code from the gdrcopy project](https://github.com/NVIDIA/gdrcopy). As such, they are licensed under [MIT license](https://github.com/NVIDIA/gdrcopy/blob/master/LICENSE)


All other files are mine, including a particularly uninspiring CBLas SGEMM, various plots, etc.
All of them are released under Apache 2 license.
Copyright, Anthony Lichnewsky, all rights reserved. (probably since 2015).
