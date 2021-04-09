# matrix_test Source files
set(matrix_test_sources
        main.cpp blaze_tests.cpp blaze_tests.hpp parametres.hpp eigen_tests.cpp eigen_tests.hpp gsl_tests.cpp gsl_tests.hpp test_runner.cpp test_runner.hpp)

# BioFM executable Target
add_executable(matrix_test ${matrix_test_sources})
target_link_libraries(matrix_test
        PRIVATE project_options project_warnings)

target_link_libraries(matrix_test PRIVATE fmt::fmt)
target_include_directories(matrix_test PRIVATE ${PCG_INCLUDE_DIRS})
target_link_libraries(matrix_test PRIVATE LAPACK::LAPACK)
target_link_libraries(matrix_test PRIVATE Eigen3::Eigen)
target_link_libraries(matrix_test PRIVATE GSL::gsl GSL::gslcblas)
target_link_libraries(matrix_test PRIVATE blaze::blaze)
target_link_libraries(matrix_test PRIVATE ${ARMADILLO_LIBRARIES})
