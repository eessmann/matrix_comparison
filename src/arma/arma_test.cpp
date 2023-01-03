//
// Created by Erich Essmann on 03/01/2023.
//
#include <armadillo>
#include <chrono>
#include <random>
#include <pcg_random.hpp>

#include "arma_test.hpp"
#include "parameters.hpp"

namespace mp = matrix_comparison::parameters;

double run_Arma_symm(const size_t size, const double alpha) {
    arma::mat const arma_pei = alpha * arma::eye(size, size) + arma::ones(size, size);
    double avg_sum = 0.0;

    // Test Loop
    for (size_t j = 0; j < mp::num_iter; j++) {
        arma::vec w(size);
        arma::mat V(size, size);
        auto start = std::chrono::steady_clock::now();
        arma::eig_sym(w, V, arma_pei);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / mp::num_iter;
}

double run_Arma_asymm(const size_t size) {
    // Specify the engine and distribution.
    static thread_local pcg64 engine{pcg_extras::seed_seq_from<std::random_device>{}};
    std::uniform_real_distribution<double> dist(-1, 1);

    auto gen = [&dist](auto) {
        return dist(engine);
    };
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < mp::num_iter; j++) {
        arma::mat const mat = arma::mat(size, size).transform(gen) + arma::eye(size, size);
        arma::cx_vec w(size);
        arma::cx_mat V(size, size);

        auto start = std::chrono::steady_clock::now();
        arma::eig_gen(w, V, mat);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }

    return avg_sum / mp::num_iter;
}
