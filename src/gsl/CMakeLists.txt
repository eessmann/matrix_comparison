# matrix test runner
add_library(gsl_runner
        gsl_tests.cpp
        gsl_tests.hpp
        )
target_link_libraries(gsl_runner
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(gsl_runner
        PRIVATE GSL::gsl GSL::gslcblas)
target_link_libraries(gsl_runner
        PRIVATE pcg-cpp::pcg-cpp)
target_link_libraries(gsl_runner
        PRIVATE parameters)