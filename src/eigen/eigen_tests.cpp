//
// Created by erich on 09/04/2021.
//
#define EIGEN_USE_BLAS

#include <chrono>
#include <Eigen/Eigenvalues>
#include <pcg_random.hpp>
#include <random>

#include "parameters.hpp"
#include "eigen_tests.hpp"

namespace mp = matrix_comparison::parameters;

double run_Eigen_symm(const size_t m_size, const double alpha) {
    const auto size = static_cast<int64_t>(m_size);
    auto const eigen_pei = Eigen::MatrixXd::Ones(size, size) + Eigen::MatrixXd::Identity(size, size) * alpha;
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < mp::num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        es.compute(eigen_pei, true);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();

        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
    }
    return avg_sum / mp::num_iter;
}

double run_Eigen_asymm(const size_t m_size) {
    // Specify the engine and distribution.
    static thread_local pcg64 engine{pcg_extras::seed_seq_from<std::random_device>{}};
    std::uniform_real_distribution<double> dist(-1, 1);

    auto gen = [&dist]() {
        return dist(engine);
    };

    const auto size = static_cast<int64_t>(m_size);
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < mp::num_iter; j++) {
        auto const mat = Eigen::MatrixXd::NullaryExpr(size, size, gen) + Eigen::MatrixXd::Identity(size, size);
        auto start = std::chrono::steady_clock::now();
        es.compute(mat, true);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();

        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
    }
    return avg_sum / mp::num_iter;
}