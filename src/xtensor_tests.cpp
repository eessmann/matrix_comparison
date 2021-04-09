//
// Created by erich on 09/04/2021.
//
#include <chrono>
#include <xtensor/xarray.hpp>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/xrandom.hpp>

#include "parametres.hpp"
#include "xtensor_tests.hpp"

double run_Xtensor_symm(const size_t m_size, const double alpha) {
    auto xten_pei = xt::ones<double>({m_size, m_size}) + xt::eye(m_size) * alpha;
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        volatile auto results = xt::linalg::eig(xten_pei);
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}

double run_Xtensor_asymm(const size_t m_size) {
    double avg_sum = 0.0;

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        auto mat = xt::random::rand<double>({m_size, m_size});
        auto start = std::chrono::steady_clock::now();
        volatile auto results= xt::linalg::eig(mat);
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}
