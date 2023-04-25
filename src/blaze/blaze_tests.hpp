//
// Created by erich on 09/04/2021.
//

#ifndef MATRIX_COMPARISON_BLAZE_TESTS_HPP
#define MATRIX_COMPARISON_BLAZE_TESTS_HPP

namespace blaze_bench {
double run_symm(const size_t size, const double alpha);
double run_asymm(const size_t size);

double run_inv_symm(const size_t size, const double alpha);
double run_inv_asymm(const size_t size);


}  // namespace blaze_bench

#endif //MATRIX_COMPARISON_BLAZE_TESTS_HPP
