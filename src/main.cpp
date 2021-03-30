#include <cstdio>
#include <complex>
#include <chrono>
#include <cstdlib>
#include <random>

#include "pcg_random.hpp"

#include <blaze/Blaze.h>

#include <Eigen/Eigenvalues>

#include <gsl/gsl_eigen.h>

constexpr size_t num_iter = 64;

double run_Blaze_symm(const int size, const double alpha) {
    blaze::SymmetricMatrix<blaze::DynamicMatrix<double>> blaze_pei = blaze::uniform(size, size, 0.0);
    blaze_pei += blaze::IdentityMatrix<double>(size) * alpha;
    double avg_sum = 0.0;

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        blaze::DynamicVector<double, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(blaze_pei, w, V);
    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        blaze::DynamicVector<double, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(blaze_pei, w, V);
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }

    return avg_sum / num_iter;
}

double run_Blaze_asymm(const int size) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    pcg64 engine(random());
    std::uniform_real_distribution<double> dist(-1, 1);

    auto gen = [&dist, &engine](size_t i, size_t j) {
        return dist(engine);
    };
    double avg_sum = 0.0;

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        blaze::DynamicMatrix<double> mat = blaze::generate(size, size, gen);
        blaze::DynamicVector<std::complex<double>, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(mat, w, V);
    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        blaze::DynamicMatrix<double> mat = blaze::generate(size, size, gen);
        auto start = std::chrono::steady_clock::now();
        blaze::DynamicVector<std::complex<double>, blaze::columnVector> w(size);
        blaze::DynamicMatrix<std::complex<double>, blaze::rowMajor> V(size, size);
        blaze::eigen(mat, w, V);
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }

    return avg_sum / num_iter;
}


double run_Eigen_symm(int size, double alpha) {
    Eigen::MatrixXd eigen_pei(size, size);
    eigen_pei.setOnes();
    eigen_pei += Eigen::MatrixXd::Identity(size, size) * alpha;
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        es.compute(eigen_pei, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        es.compute(eigen_pei, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}

double run_GSL_symm(int size, double alpha) {
    double data[size * size];
    for (int i = 0; i < size * size; i++) {
        data[i] = 1.0;
    }
    for (int i = 0; i < size; i++) {
        data[i + i * size] += alpha;
    }

    double avg_sum = 0.0;
    gsl_matrix_view gsl_pei = gsl_matrix_view_array(data, size, size);

    gsl_vector *eval = gsl_vector_alloc(size);
    gsl_matrix *evec = gsl_matrix_alloc(size, size);
    gsl_eigen_symmv_workspace *wspace = gsl_eigen_symmv_alloc(size);

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        gsl_eigen_symmv(&gsl_pei.matrix, eval, evec, wspace);
    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        auto start = std::chrono::steady_clock::now();
        gsl_eigen_symmv(&gsl_pei.matrix, eval, evec, wspace);
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }
    gsl_eigen_symmv_free(wspace);
    return avg_sum / num_iter;
}

double run_Eigen_asymm(int size) {
    Eigen::EigenSolver<Eigen::MatrixXd> es;
    double avg_sum = 0.0;

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        Eigen::MatrixXd mat(size, size);
        mat.setRandom();
        mat += Eigen::MatrixXd::Identity(size, size);

        es.compute(mat, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();

    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        Eigen::MatrixXd mat(size, size);
        mat.setRandom();
        mat += Eigen::MatrixXd::Identity(size, size);
        auto start = std::chrono::steady_clock::now();
        es.compute(mat, true);
        volatile Eigen::MatrixXcd evectors = es.eigenvectors();
        volatile Eigen::MatrixXcd evalues = es.eigenvalues();
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }
    return avg_sum / num_iter;
}

double run_GSL_asymm(int size) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    pcg64 engine(random());
    std::uniform_real_distribution<double> dist(-1, 1);
    auto gen = [&dist, &engine]() {
        return dist(engine);
    };

    double data[size * size];

    double avg_sum = 0.0;

    gsl_vector_complex *eval = gsl_vector_complex_alloc(size);
    gsl_matrix_complex *evec = gsl_matrix_complex_alloc(size, size);
    gsl_eigen_nonsymmv_workspace *wspace = gsl_eigen_nonsymmv_alloc(size);

    // Warm-up Cycle
    for (int j = 0; j < num_iter; j++) {
        for (int i = 0; i < size * size; i++) {
            data[i] = gen();
        }
        gsl_matrix_view gsl_pei = gsl_matrix_view_array(data, size, size);

        gsl_eigen_nonsymmv(&gsl_pei.matrix, eval, evec, wspace);
    }

    // Test Cycle
    for (int j = 0; j < num_iter; j++) {
        for (int i = 0; i < size * size; i++) {
            data[i] = gen();
        }
        gsl_matrix_view gsl_pei = gsl_matrix_view_array(data, size, size);

        auto start = std::chrono::steady_clock::now();
        gsl_eigen_nonsymmv(&gsl_pei.matrix, eval, evec, wspace);
        avg_sum += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start).count();
    }
    gsl_eigen_nonsymmv_free(wspace);
    return avg_sum / num_iter;
}


int main() {
    FILE *symm_fp = fopen("csv_symm_results.csv", "w");
    printf("size,\tEigen,\tBlaze,\tGSL\n");
    fprintf(symm_fp, "size,\tEigen,\tTNT,\tGSL\n");
    for (int size = 5; size < 200; size += 1) {
        double blaze_time = run_Blaze_symm(size, 0.4);
        double eigen_time = run_Eigen_symm(size, 0.4);
        double gsl_time = run_GSL_symm(size, 0.4);
        printf("%*d,%*.2f,%*.2f,%*.2f\n", 7, size, 12, eigen_time, 12, blaze_time, 12, gsl_time);
        fprintf(symm_fp, "%*d,%*.2f,%*.2f,%*.2f\n", 7, size, 12, eigen_time, 12, blaze_time, 12, gsl_time);
    }
    fclose(symm_fp);

    FILE *asymm_fp = fopen("csv_asymm_results.csv", "w");
    printf("size,\tEigen,\tBlaze,\tGSL\n");
    fprintf(asymm_fp, "size,\tEigen,\tBlaze,\tGSL\n");
    for (int size = 5; size < 200; size += 1) {
        double blaze_time = run_Blaze_asymm(size);
        double eigen_time = run_Eigen_asymm(size);
        double gsl_time = run_GSL_asymm(size);
        printf("%*d,%*.2f,%*.2f,%*.2f\n", 7, size, 12, eigen_time, 12, blaze_time, 12, gsl_time);
        fprintf(asymm_fp, "%*d,%*.2f,%*.2f,%*.2f\n", 7, size, 12, eigen_time, 12, blaze_time, 12, gsl_time);
    }
    fclose(asymm_fp);
}

