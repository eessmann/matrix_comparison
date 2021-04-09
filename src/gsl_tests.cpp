//
// Created by erich on 09/04/2021.
//
#include <vector>
#include <chrono>
#include <random>

#include "parametres.hpp"
#include "pcg_random.hpp"

#include <gsl/gsl_eigen.h>
#include "gsl_tests.hpp"

double run_GSL_symm(const size_t size, const double alpha) {

    std::vector<double> data(size * size, 1.0);

    for (size_t i = 0; i < size; i++) {
        data[i + i * size] += alpha;
    }

    double avg_sum = 0.0;
    gsl_matrix_view gsl_pei = gsl_matrix_view_array(data.data(), size, size);

    gsl_vector *eval = gsl_vector_alloc(size);
    gsl_matrix *evec = gsl_matrix_alloc(size, size);
    gsl_eigen_symmv_workspace *wspace = gsl_eigen_symmv_alloc(size);

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        gsl_eigen_symmv(&gsl_pei.matrix, eval, evec, wspace);
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    gsl_eigen_symmv_free(wspace);
    return avg_sum / num_iter;
}

double run_GSL_asymm(const size_t size) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    pcg64 engine(static_cast<__uint128_t>(random()));
    std::uniform_real_distribution<double> dist(-1, 1);
    auto gen = [&dist, &engine]() {
        return dist(engine);
    };

    std::vector<double> data(size * size);
    double avg_sum = 0.0;

    gsl_vector_complex *eval = gsl_vector_complex_alloc(size);
    gsl_matrix_complex *evec = gsl_matrix_complex_alloc(size, size);
    gsl_eigen_nonsymmv_workspace *wspace = gsl_eigen_nonsymmv_alloc(size);

    // Test Cycle
    for (size_t j = 0; j < num_iter; j++) {
        std::generate(data.begin(), data.end(), gen);
        gsl_matrix_view gsl_pei = gsl_matrix_view_array(data.data(), size, size);

        auto start = std::chrono::steady_clock::now();
        gsl_eigen_nonsymmv(&gsl_pei.matrix, eval, evec, wspace);
        avg_sum += std::chrono::duration_cast<microsecond_d>(
                std::chrono::steady_clock::now() - start).count();
    }
    gsl_eigen_nonsymmv_free(wspace);
    return avg_sum / num_iter;
}
