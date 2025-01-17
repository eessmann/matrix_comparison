//
// Created by erich on 09/04/2021.
//
#include <vector>
#include <chrono>
#include <random>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>
#include <pcg_random.hpp>
#include <range/v3/algorithm/generate.hpp>

#include "parameters.hpp"
#include "gsl_tests.hpp"

namespace gsl_bench {
namespace mp = matrix_comparison::parameters;

double run_symm(const size_t size, const double alpha) {
  std::vector<double> data(size * size, 1.0);

  for (size_t i = 0; i < size; i++) {
    data[i + i * size] += alpha;
  }

  double avg_sum = 0.0;
  gsl_matrix_view gsl_pei = gsl_matrix_view_array(data.data(), size, size);

  gsl_vector* eval = gsl_vector_alloc(size);
  gsl_matrix* evec = gsl_matrix_alloc(size, size);
  gsl_eigen_symmv_workspace* wspace = gsl_eigen_symmv_alloc(size);

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    auto start = std::chrono::steady_clock::now();
    gsl_eigen_symmv(&gsl_pei.matrix, eval, evec, wspace);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::steady_clock::now() - start)
                   .count();
  }
  gsl_vector_free(eval);
  gsl_matrix_free(evec);
  gsl_eigen_symmv_free(wspace);
  return avg_sum / mp::num_iter;
}

double run_inv_symm(const size_t size, const double alpha) {
  std::vector<double> data(size * size, 1.0);

  for (size_t i = 0; i < size; i++) {
    data[i + i * size] += alpha;
  }

  double avg_sum = 0.0;
  auto gsl_pei = gsl_matrix_view_array(data.data(), size, size);
  int s;

  auto* inv_mat = gsl_matrix_alloc(size, size);
  auto* wspace = gsl_permutation_alloc(size);

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    auto start = std::chrono::steady_clock::now();
    gsl_linalg_LU_decomp(&gsl_pei.matrix, wspace, &s);
    gsl_linalg_LU_invert(&gsl_pei.matrix, wspace, inv_mat);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::steady_clock::now() - start)
                   .count();
  }
  gsl_matrix_free(inv_mat);
  gsl_permutation_free(wspace);
  return avg_sum / mp::num_iter;
}

double run_asymm(const size_t size) {
  // Specify the engine and distribution.
  pcg64 engine{
      pcg_extras::seed_seq_from<std::random_device>{}};
  std::uniform_real_distribution<double> dist(-1, 1);

  auto gen = [&dist, &engine]() { return dist(engine); };

  std::vector<double> data(size * size);
  double avg_sum = 0.0;

  gsl_vector_complex* eval = gsl_vector_complex_alloc(size);
  gsl_matrix_complex* evec = gsl_matrix_complex_alloc(size, size);
  gsl_eigen_nonsymmv_workspace* wspace = gsl_eigen_nonsymmv_alloc(size);

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    ranges::generate(data, gen);
    for (size_t i = 0; i < size; i++) {
      data[i + i * size] += 1;
    }
    gsl_matrix_view gsl_pei = gsl_matrix_view_array(data.data(), size, size);

    auto start = std::chrono::high_resolution_clock::now();
    gsl_eigen_nonsymmv(&gsl_pei.matrix, eval, evec, wspace);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();
  }
  gsl_vector_complex_free(eval);
  gsl_matrix_complex_free(evec);
  gsl_eigen_nonsymmv_free(wspace);
  return avg_sum / mp::num_iter;
}

double run_inv_asymm(const size_t size) {
  // Specify the engine and distribution.
  pcg64 engine{
      pcg_extras::seed_seq_from<std::random_device>{}};
  std::uniform_real_distribution<double> dist(-1, 1);

  auto gen = [&dist, &engine]() { return dist(engine); };

  std::vector<double> data(size * size);
  double avg_sum = 0.0;
  int s;

  auto* inv_mat = gsl_matrix_alloc(size, size);
  auto* wspace = gsl_permutation_alloc(size);

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    ranges::generate(data, gen);
    for (size_t i = 0; i < size; i++) {
      data[i + i * size] += 1;
    }
    gsl_matrix_view gsl_pei = gsl_matrix_view_array(data.data(), size, size);

    auto start = std::chrono::high_resolution_clock::now();
    gsl_linalg_LU_decomp(&gsl_pei.matrix, wspace, &s);
    gsl_linalg_LU_invert(&gsl_pei.matrix, wspace, inv_mat);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();
  }
  gsl_matrix_free(inv_mat);
  gsl_permutation_free(wspace);
  return avg_sum / mp::num_iter;
}

}  // namespace gsl_bench