# matrix_test Source files
set(matrix_test_sources
        main.cpp)

# BioFM executable Target
add_executable(matrix_test ${matrix_test_sources})
target_link_libraries(matrix_test
        PRIVATE project_options project_warnings)

target_include_directories(matrix_test PRIVATE ${PCG_INCLUDE_DIRS})
target_link_libraries(matrix_test PUBLIC LAPACK::LAPACK)
target_link_libraries(matrix_test PUBLIC Eigen3::Eigen)
target_link_libraries(matrix_test PUBLIC GSL::gsl GSL::gslcblas)
target_link_libraries(matrix_test PUBLIC blaze::blaze)
target_link_libraries(matrix_test PUBLIC ${ARMADILLO_LIBRARIES})
