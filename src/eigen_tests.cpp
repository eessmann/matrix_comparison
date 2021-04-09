//
// Created by erich on 09/04/2021.
//
#define EIGEN_USE_BLAS
#include <chrono>
#include <Eigen/Eigenvalues>

#include "parametres.hpp"
#include "eigen_tests.hpp"

double run_Eigen_symm(const size_t m_size, const double alpha) {
    const auto size = static_cast<int64_t>(m_size);
    Eigen::MatrixXd eigen_pei(size, size);
    eigen_pei.setOnes();
    eigen_pei += Eigen::MatrixXd::Identity(size, size) * alpha;
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        es.compute(eigen_pei, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}

double run_Eigen_asymm(const size_t m_size) {
    const auto size = static_cast<int64_t>(m_size);
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        Eigen::MatrixXd mat(size, size);
        mat.setRandom();
        mat += Eigen::MatrixXd::Identity(size, size);
        auto start = std::chrono::steady_clock::now();
        es.compute(mat, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}