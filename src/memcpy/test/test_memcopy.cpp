/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../memcopy.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <benchmark/benchmark.h>


#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

static void BM_naive(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* src = new char[n];
  char* dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){

      for( std::size_t i = 0; i < n; ++i){
          dst[i] = src[i];
      }
      //benchmark::DoNotOptimize( dst[n-1] );
  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_naive)->Range(1 << 24, 1 << 27 );

static void BM_naive2(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* src = new char[n];
  char* dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){
      char *s=src, *d=dst;
      for( std::size_t i = 0; i < n; ++i){
          *d++ = *s++;
      }
      benchmark::DoNotOptimize( dst[n-1] );
  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_naive2)->Range(1 << 26, 1 << 27 );



static void BM_memcpy(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* __restrict src = new char[n];
  char* __restrict dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){

    std::memcpy(dst, src, n);

  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy)->Range(64, 1 << 28 );

static void BM_memcpy_uncached_store_avx(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* __restrict src = reinterpret_cast<char*>(aligned_alloc( 16, n ));
  char* __restrict dst = reinterpret_cast<char*>(aligned_alloc( 16, n ));

  std::memset(src, 'x', n);

  for (auto _ : state){

    memcpy_uncached_store_avx(dst, src, n);
    // benchmark::DoNotOptimize( dst[n-1] );
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  free(src);
  free(dst);

}
BENCHMARK(BM_memcpy_uncached_store_avx)->Range(64, 1 << 28 );

static void BM_memcpy_uncached_store_sse(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* src = new char[n];
  char* dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){

    memcpy_uncached_store_sse(dst, src, n);

  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy_uncached_store_sse)->Range(64, 1 << 28 );


static void BM_memcpy_cached_store_avx(benchmark::State& state) {
  std::size_t n = state.range(0);
  //char* __restrict src = new char[n];
  //char* __restrict dst = new char[n];
  char* __restrict src = reinterpret_cast<char*> (aligned_alloc(16, n));
  char* __restrict dst = reinterpret_cast<char*> (aligned_alloc(16, n));

  std::memset(src, 'x', n);

  for (auto _ : state){
    memcpy_cached_store_avx(dst, src, n);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  free(src);
  free(dst);
  //delete[] src;
  //delete[] dst;
}
BENCHMARK(BM_memcpy_cached_store_avx)->Range(64, 1 << 28 );

static void BM_memcpy_cached_store_sse(benchmark::State& state) {

  std::size_t n = state.range(0);

  char* __restrict src = new char[n];
  char* __restrict dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){
     memcpy_cached_store_sse(dst, src, n);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy_cached_store_sse)->Range(64, 1 << 28 );


static void BM_memcpy_uncached_load_sse41(benchmark::State& state) {
  std::size_t n = state.range(0);
  char* __restrict src = new char[n];
  char* __restrict dst = new char[n];

  std::memset(src, 'x', n);

  for (auto _ : state){
    memcpy_uncached_load_sse41(dst, src, n);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(n));

  benchmark::DoNotOptimize( dst[n-1] );

  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy_uncached_load_sse41)->Range(64, 1 << 28 );

// Ensure that StateIterator provides all the necessary typedefs required to
// instantiate std::iterator_traits.
static_assert(std::is_same<
  typename std::iterator_traits<benchmark::State::StateIterator>::value_type,
  typename benchmark::State::StateIterator::value_type>::value, "");

BENCHMARK_MAIN();
