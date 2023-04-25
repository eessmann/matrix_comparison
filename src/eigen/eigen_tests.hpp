//
// Created by erich on 09/04/2021.
//

#ifndef MATRIX_COMPARISON_EIGEN_TESTS_HPP
#define MATRIX_COMPARISON_EIGEN_TESTS_HPP

namespace eigen_bench {
double run_symm(const size_t m_size, double const alpha);
double run_asymm(const size_t m_size);

double run_inv_symm(const size_t m_size, double const alpha);
double run_inv_asymm(const size_t m_size);
}  // namespace eigen_bench
#endif  // MATRIX_COMPARISON_EIGEN_TESTS_HPP
