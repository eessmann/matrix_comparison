//
// Created by erich on 09/04/2021.
//
#include <fstream>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <tuple>
#include <ranges>
#include <spdlog/spdlog.h>

#include "parametres.hpp"
#include "blaze_tests.hpp"
#include "eigen_tests.hpp"
#include "gsl_tests.hpp"

#include "test_runner.hpp"

namespace ranges = std::ranges;

template<typename Lambda>
void run_bench(Lambda ben, std::filesystem::path &filename) {
    spdlog::info("size,\tEigen,\tBlaze,\tGSL");
    std::vector<std::tuple<size_t, double, double, double>> results(size_table.size());
    ranges::transform(size_table.begin(), size_table.end(), results.begin(), ben);

    std::ofstream times(filename);
    fmt::print(times, "size,\tEigen,\tBlaze,\tGSL\n");
    for (auto r : results) {
        fmt::print(times, "{}\n", fmt::join(r, ",\t"));
    }
}

void symmetric_eigen_bench() {
    std::filesystem::path sym_file = fmt::format("csv_symm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                 fmt::localtime(std::chrono::system_clock::now()));

    auto bench = [](auto size) {
        double blaze_time = run_Blaze_symm(size, 0.4);
        double eigen_time = run_Eigen_symm(size, 0.4);
        double gsl_time = run_GSL_symm(size, 0.4);
        spdlog::info("{},\t{:.2f},\t{:.2f},\t{:.2f}", size, eigen_time, blaze_time, gsl_time);
        return std::make_tuple(size, eigen_time, blaze_time, gsl_time);
    };
    run_bench(bench, sym_file);
}

void asymmetric_eigen_bench() {
    std::filesystem::path asym_file = fmt::format("csv_asymm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                  fmt::localtime(std::chrono::system_clock::now()));

    auto bench = [](auto size) {
        double blaze_time = run_Blaze_asymm(size);
        double eigen_time = run_Eigen_asymm(size);
        double gsl_time = run_GSL_asymm(size);
        spdlog::info("{},\t{:.2f},\t{:.2f},\t{:.2f}", size, eigen_time, blaze_time, gsl_time);
        return std::make_tuple(size, eigen_time, blaze_time, gsl_time);
    };
    run_bench(bench, asym_file);
}
