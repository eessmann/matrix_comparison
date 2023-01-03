//
// Created by erich on 09/04/2021.
//
#include <complex>
#include <chrono>
#include <random>
#include <blaze/Blaze.h>
#include <pcg_random.hpp>

#include "blaze_tests.hpp"
#include "parameters.hpp"

namespace mp = matrix_comparison::parameters;

double run_Blaze_symm(const size_t size, const double alpha) {
    blaze::SymmetricMatrix<blaze::DynamicMatrix<double>> const blaze_pei =
            blaze::uniform(size, size, 1.0) + blaze::IdentityMatrix<double>(size) * alpha;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < mp::num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        blaze::DynamicVector<double, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(blaze_pei, w, V);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }

    return avg_sum / mp::num_iter;
}

double run_Blaze_asymm(const size_t size) {

    // Specify the engine and distribution.
    pcg64 engine{pcg_extras::seed_seq_from<std::random_device>{}};
    std::uniform_real_distribution<double> dist(-1, 1);

    auto gen = [&dist, &engine](...) {
        return dist(engine);
    };
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < mp::num_iter; j++) {
        blaze::DynamicMatrix<double> mat = blaze::generate(size, size, gen);
        auto start = std::chrono::steady_clock::now();
        blaze::DynamicVector<std::complex<double>, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(mat, w, V);
        avg_sum += std::chrono::duration_cast<mp::microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }

    return avg_sum / mp::num_iter;
}