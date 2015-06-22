


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

if("${project_GOOGLE_TEST_PROVIDER}" STREQUAL "module")
  if(NOT GOOGLE_TEST_ROOT_DIR)
    set(GOOGLE_TEST_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest)
  endif()
  if(EXISTS "${GOOGLE_TEST_ROOT_DIR}/CMakeLists.txt")
      add_subdirectory(${GOOGLE_TEST_ROOT_DIR} third_party/googletest)
      if(TARGET GOOGLE_TEST)
          set(GTEST_LIBRARIES gtest)
          set(GTEST_INCLUDE_DIR "${GOOGLE_TEST_ROOT_DIR}/include")
      endif()
  else()
      message(WARNING "project_GOOGLE_TEST_PROVIDER is \"module\" but GOOGLE_TEST_ROOT_DIR is wrong")
  endif()
elseif("${project_GOOGLE_TEST_PROVIDER}" STREQUAL "package")
  # Use "CONFIG" as there is no built-in cmake module for gtest.
  find_package(GTest REQUIRED CONFIG)
  if(TARGET gtest::gtest)
    set(_project_gtest_LIBRARIES gtest::gtest)
    # extract the include dir from target's properties
    get_target_property(_project_gtest_INCLUDE_DIR gtest::gtest INTERFACE_INCLUDE_DIRECTORIES)
  endif()
  set(_project_FIND_gtest "if(NOT gtest_FOUND)\n  find_package(GTest CONFIG)\nendif()")
endif()
