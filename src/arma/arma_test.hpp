//
// Created by Erich Essmann on 03/01/2023.
//

#ifndef MATRIX_COMPARISON_ARMA_TEST_H
#define MATRIX_COMPARISON_ARMA_TEST_H
#include <cstdlib>

namespace arma_bench {

double run_symm(const size_t size, double const alpha);
double run_asymm(const size_t size);

double run_inv_symm(const size_t size, double const alpha);
double run_inv_asymm(const size_t size);

}  // namespace arma_bench
#endif  // MATRIX_COMPARISON_ARMA_TEST_H
