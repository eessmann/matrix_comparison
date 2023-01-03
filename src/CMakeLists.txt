# parameters
add_subdirectory(parameters)

# matrix libraries
add_subdirectory(arma)
add_subdirectory(blaze)
add_subdirectory(eigen)
add_subdirectory(gsl)
add_subdirectory(xtensor)

# matrix test runner
add_library(matrix_test_runner
    test_runner.cpp
    test_runner.hpp
    )
target_link_libraries(matrix_test_runner
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(matrix_test_runner PRIVATE fmt::fmt)
target_link_libraries(matrix_test_runner PRIVATE spdlog::spdlog)
target_link_libraries(matrix_test_runner PRIVATE parameters)
target_link_libraries(matrix_test_runner PRIVATE blaze_runner)
target_link_libraries(matrix_test_runner PRIVATE eigen_runner)
target_link_libraries(matrix_test_runner PRIVATE gsl_runner)

# matrix_test executable Target
add_executable(matrix_test main.cpp)
target_link_libraries(matrix_test
        PRIVATE matrix_comparison::project_options matrix_comparison::project_warnings)
target_link_libraries(matrix_test PRIVATE matrix_test_runner)
