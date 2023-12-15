//
// Created by erich on 09/04/2021.
//
#define EIGEN_USE_BLAS

#include <Eigen/Eigenvalues>
#include <chrono>
#include <pcg_random.hpp>
#include <random>

#include "eigen_tests.hpp"
#include "parameters.hpp"
namespace eigen_bench {
namespace mp = matrix_comparison::parameters;

double run_symm(const size_t m_size, double const alpha) {
  auto const size = static_cast<int64_t>(m_size);
  auto const eigen_pei = Eigen::MatrixXd::Ones(size, size) +
                         Eigen::MatrixXd::Identity(size, size) * alpha;
  Eigen::EigenSolver<Eigen::MatrixXd> es;
  double avg_sum = 0.0;

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    auto start = std::chrono::high_resolution_clock::now();
    es.compute(eigen_pei, true);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();

    volatile Eigen::MatrixXcd evectors = es.eigenvectors();
    volatile Eigen::MatrixXcd evalues = es.eigenvalues();
  }
  return avg_sum / mp::num_iter;
}

double run_inv_symm(const size_t m_size, double const alpha) {
  auto const size = static_cast<int64_t>(m_size);
  auto const eigen_pei = Eigen::MatrixXd::Ones(size, size) +
                         Eigen::MatrixXd::Identity(size, size) * alpha;
  double avg_sum = 0.0;
  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    auto start = std::chrono::high_resolution_clock::now();
    volatile Eigen::MatrixXd const eigen_pei_inv = eigen_pei.inverse();
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();
  }
  return avg_sum / mp::num_iter;
}

double run_asymm(const size_t m_size) {
  // Specify the engine and distribution.
  pcg64 engine{
      pcg_extras::seed_seq_from<std::random_device>{}};
  std::uniform_real_distribution<double> dist(-1, 1);

  auto gen = [&dist, &engine]() { return dist(engine); };

  auto const size = static_cast<int64_t>(m_size);
  Eigen::EigenSolver<Eigen::MatrixXd> es;
  double avg_sum = 0.0;

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    auto const mat = Eigen::MatrixXd::NullaryExpr(size, size, gen) +
                     Eigen::MatrixXd::Identity(size, size);
    auto start = std::chrono::high_resolution_clock::now();
    es.compute(mat, true);
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();

    volatile Eigen::MatrixXcd evectors = es.eigenvectors();
    volatile Eigen::MatrixXcd evalues = es.eigenvalues();
  }
  return avg_sum / mp::num_iter;
}

double run_inv_asymm(const size_t m_size) {
  // Specify the engine and distribution.
  pcg64 engine{
      pcg_extras::seed_seq_from<std::random_device>{}};
  std::uniform_real_distribution<double> dist(-1, 1);

  auto gen = [&dist, &engine]() -> double { return dist(engine); };

  auto const size = static_cast<int64_t>(m_size);
  double avg_sum = 0.0;

  // Test Cycle
  for (size_t j = 0; j < mp::num_iter; j++) {
    Eigen::MatrixXd const mat = Eigen::MatrixXd::NullaryExpr(size, size, gen) +
                                Eigen::MatrixXd::Identity(size, size);
    auto start = std::chrono::high_resolution_clock::now();
    volatile Eigen::MatrixXd const mat_inv = mat.inverse();
    avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count();
  }
  return avg_sum / mp::num_iter;
}
}  // namespace eigen_bench