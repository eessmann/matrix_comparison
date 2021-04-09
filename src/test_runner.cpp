//
// Created by erich on 09/04/2021.
//
#include <fstream>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/ostream.h>

#include "parametres.hpp"
#include "blaze_tests.hpp"
#include "eigen_tests.hpp"
#include "gsl_tests.hpp"

#include "test_runner.hpp"

void symmetric_eigen_bench() {
    std::filesystem::path sym_file = fmt::format("csv_symm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                 fmt::localtime(std::chrono::system_clock::now()));
    std::ofstream symm_times(sym_file);
    fmt::print("size,\tEigen,\tBlaze,\tGSL\n");
    fmt::print(symm_times, "size,\tEigen,\tBlaze,\tGSL\n");
    for(const auto iter : size_table) {
        double blaze_time = run_Blaze_symm(iter, 0.4);
        double eigen_time = run_Eigen_symm(iter, 0.4);
        double gsl_time = run_GSL_symm(iter, 0.4);
        fmt::print("{},\t{:.2f},\t{:.2f},\t{:.2f}\n", iter, eigen_time, blaze_time, gsl_time);
        fmt::print(symm_times, "{},{},{},{}\n", iter, eigen_time, blaze_time, gsl_time);

    }
}

void asymmetric_eigen_bench() {
    std::filesystem::path asym_file = fmt::format("csv_asymm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                  fmt::localtime(std::chrono::system_clock::now()));
    std::ofstream asymm_fp(asym_file);
    fmt::print("size,\tEigen,\tBlaze,\tGSL\n");
    fmt::print(asymm_fp, "size,\tEigen,\tBlaze,\tGSL\n");
    for (const auto iter : size_table) {
        double blaze_time = run_Blaze_asymm(iter);
        double eigen_time = run_Eigen_asymm(iter);
        double gsl_time = run_GSL_asymm(iter);
        fmt::print("{},\t{:.2f},\t{:.2f},\t{:.2f}\n", iter, eigen_time, blaze_time, gsl_time);
        fmt::print(asymm_fp, "{},{},{},{}\n", iter, eigen_time, blaze_time, gsl_time);
    }
}
