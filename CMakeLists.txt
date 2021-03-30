cmake_minimum_required(VERSION 3.15...3.19)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")
include(CMakePrintHelpers)

project(matrix_math_comparison CXX)

# Link this 'library' to set the c++ standard / compile-time options requested
set(CMAKE_CXX_EXTENSIONS OFF)
# Fixing APPLE CXX standard bug
if(APPLE)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(LAPACK CONFIG REQUIRED)

find_package(blaze CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)


add_executable(matrix_test
        src/main.cpp
        )

find_path(PCG_INCLUDE_DIRS "pcg_extras.hpp")
target_include_directories(matrix_test PRIVATE ${PCG_INCLUDE_DIRS})

target_link_libraries(matrix_test PUBLIC LAPACK::LAPACK)
target_link_libraries(matrix_test PUBLIC Eigen3::Eigen)
target_link_libraries(matrix_test PUBLIC GSL::gsl GSL::gslcblas)
target_link_libraries(matrix_test PUBLIC blaze::blaze)



