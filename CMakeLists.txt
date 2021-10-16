cmake_minimum_required(VERSION 3.14)
project(my_project)
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Download and unpack googletest at configure time
configure_file(build_support/googletest/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()


set(source_dir src/)
set(include_dir src/include/)
set(utils_module_dir ${source_dir}utils/)
set(bloom_dir ${source_dir}filter/)
set(test_dir tests/)


file(GLOB utils_file ${utils_module_dir}*.cc)
ADD_LIBRARY(utils
  SHARED
  ${utils_file}
)
TARGET_INCLUDE_DIRECTORIES(utils
# interface means not the lib
  PUBLIC
    ${include_dir}
)
ADD_LIBRARY(bloom
  SHARED
  ${bloom_dir}bloom_filter.cc
)
TARGET_INCLUDE_DIRECTORIES(bloom
# interface means not the lib
  PUBLIC
    ${include_dir}
)

TARGET_LINK_LIBRARIES(bloom utils)



file(GLOB test_file ${test_dir}*.cc)
ADD_EXECUTABLE(all_tests ${test_file})
TARGET_LINK_LIBRARIES(all_tests utils bloom gtest gtest_main)

ADD_EXECUTABLE(filter_test ${test_dir}filter_test.cc)
TARGET_LINK_LIBRARIES(filter_test bloom gtest gtest_main)

ADD_EXECUTABLE(bitset_test ${test_dir}bitset_test.cc)
TARGET_LINK_LIBRARIES(bitset_test utils gtest gtest_main)

ADD_EXECUTABLE(fileio_test ${test_dir}fileio_test.cc)
TARGET_LINK_LIBRARIES(fileio_test utils gtest gtest_main)


# The below configuration enables testing in CMake, 
# declares the C++ test binary you want to build (hello_test), 
# and links it to GoogleTest (gtest_main). 
enable_testing()
