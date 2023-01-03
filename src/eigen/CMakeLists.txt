# matrix test runner
add_library(eigen_runner
        eigen_tests.cpp
        eigen_tests.hpp
        )
target_link_libraries(eigen_runner
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(eigen_runner
        PRIVATE BLAS::BLAS)
target_link_libraries(eigen_runner
        PRIVATE Eigen3::Eigen)
target_link_libraries(eigen_runner
        PRIVATE pcg-cpp::pcg-cpp)
target_link_libraries(eigen_runner
        PRIVATE parameters)