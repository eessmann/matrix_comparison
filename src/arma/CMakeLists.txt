# matrix test runner
add_library(arma_runner
        arma_test.cpp
        arma_test.hpp
        )
target_link_libraries(arma_runner
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(arma_runner
        PRIVATE BLAS::BLAS LAPACK::LAPACK)
target_link_libraries(arma_runner
        PRIVATE armadillo::armadillo)
target_link_libraries(arma_runner
        PRIVATE pcg-cpp::pcg-cpp)
target_link_libraries(arma_runner
        PRIVATE parameters)