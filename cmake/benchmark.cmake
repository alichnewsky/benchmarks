# Copyright 2017 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if("${project_GOOGLE_BENCHMARK_PROVIDER}" STREQUAL "module")
  if(NOT GOOGLE_BENCHMARK_ROOT_DIR)
    set(GOOGLE_BENCHMARK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/benchmark)
  endif()
  if(EXISTS "${GOOGLE_BENCHMARK_ROOT_DIR}/CMakeLists.txt")
      add_subdirectory(${GOOGLE_BENCHMARK_ROOT_DIR} third_party/benchmark)
      if(TARGET benchmark)
          set(_project_GOOGLE_BENCHMARK_LIBRARIES GOOGLE_BENCHMARK)
          set(_project_GOOGLE_BENCHMARK_INCLUDE_DIR "${GOOGLE_BENCHMARK_ROOT_DIR}/include")
      endif()
  else()
      message(WARNING "project_GOOGLE_BENCHMARK_PROVIDER is \"module\" but GOOGLE_BENCHMARK_ROOT_DIR is wrong")
  endif()
elseif("${project_GOOGLE_BENCHMARK_PROVIDER}" STREQUAL "package")
  # Use "CONFIG" as there is no built-in cmake module for GOOGLE_BENCHMARK.
  find_package(GOOGLE_BENCHMARK REQUIRED CONFIG)
  if(TARGET benchmark::benchmark)
    set(_project_GOOGLE_BENCHMARK_LIBRARIES benchmark::benchmark)
    # extract the include dir from target's properties
    get_target_property(_project_GOOGLE_BENCHMARK_INCLUDE_DIR benchmark::benchmark INTERFACE_INCLUDE_DIRECTORIES)
  endif()
  set(_project_FIND_GOOGLE_BENCHMARK "if(NOT GOOGLE_BENCHMARK_FOUND)\n  find_package(GOOGLE_BENCHMARK CONFIG)\nendif()")
endif()
