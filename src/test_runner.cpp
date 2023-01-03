//
// Created by erich on 09/04/2021.
//
#include <filesystem>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <tuple>
#include <type_traits>
#include <spdlog/spdlog.h>

#include "parameters.hpp"
#include "blaze/blaze_tests.hpp"
#include "eigen/eigen_tests.hpp"
#include "gsl/gsl_tests.hpp"

#include "test_runner.hpp"

namespace ranges = std::ranges;
namespace mp = matrix_comparison::parameters;

const auto header = fmt::format("size, Eigen, Blaze, GSL");

template<typename F>
concept Function = requires(F) {
    std::is_function_v<F>;
};

template<Function Bench>
void run_bench(Bench bench, std::filesystem::path &filename) {
    using TResult = std::invoke_result_t<Bench, size_t>;
    spdlog::info("{}", header);
    std::vector<TResult> results(mp::size_table.size());
    std::transform(mp::size_table.begin(), mp::size_table.end(), results.begin(), bench);

    std::ofstream times(filename);
    fmt::print(times, "{}\n", header);
    for (auto r: results) {
        fmt::print(times, "{}\n", fmt::join(r, ", "));
    }
}

void symmetric_eigen_bench() {
    std::filesystem::path sym_file = fmt::format("csv_symm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                 fmt::localtime(std::chrono::system_clock::now()));

    auto bench = [](auto size) {
        double blaze_time = run_Blaze_symm(size, 0.4);
        double eigen_time = run_Eigen_symm(size, 0.4);
        double gsl_time = run_GSL_symm(size, 0.4);
        //double xten_time = run_Xtensor_symm(size, 0.4);
        spdlog::info("{}, {:.2f}, {:.2f}, {:.2f}", size, eigen_time, blaze_time, gsl_time);
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
        //double xten_time = run_Xtensor_asymm(size);
        spdlog::info("{}, {:.2f}, {:.2f}, {:.2f}", size, eigen_time, blaze_time, gsl_time);
        return std::make_tuple(size, eigen_time, blaze_time, gsl_time);
    };
    run_bench(bench, asym_file);
}
