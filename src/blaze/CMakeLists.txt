# matrix test runner
add_library(blaze_runner
        blaze_tests.cpp
        blaze_tests.hpp
        )
target_link_libraries(blaze_runner
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(blaze_runner
        PRIVATE BLAS::BLAS LAPACK::LAPACK)
target_link_libraries(blaze_runner
        PRIVATE blaze::blaze)
target_link_libraries(blaze_runner
        PRIVATE pcg-cpp::pcg-cpp)
target_link_libraries(blaze_runner
        PRIVATE parameters)