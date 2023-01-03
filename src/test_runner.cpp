//
// Created by erich on 09/04/2021.
//
#include <concepts>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <tuple>
#include <type_traits>
#include <spdlog/spdlog.h>
#include <range/v3/all.hpp>
#include <string_view>

#include "parameters.hpp"
#include "arma/arma_test.hpp"
#include "blaze/blaze_tests.hpp"
#include "eigen/eigen_tests.hpp"
#include "gsl/gsl_tests.hpp"

#include "test_runner.hpp"

namespace mp = matrix_comparison::parameters;

static constexpr auto header = std::string_view ("size, arma, blaze, eigen, gsl");

template<typename F>
concept Function = requires(F) {
    std::is_function_v<F>;
};

template<Function Bench>
void run_bench(Bench bench, std::filesystem::path const &filename) {
    spdlog::info("{}", header);
    auto results = ranges::views::transform(mp::size_table, bench);

    std::ofstream times(filename);
    fmt::print(times, "{}\n", header);
    ranges::for_each(results, [&times](auto result) { fmt::print(times, "{}\n", fmt::join(result, ", ")); });

}

void symmetric_eigen_bench() {
    std::filesystem::path sym_file = fmt::format("csv_symm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                 fmt::localtime(std::chrono::system_clock::now()));

    auto bench = [](auto size) {
        double arma_time = run_Arma_symm(size, 0.4);
        double blaze_time = run_Blaze_symm(size, 0.4);
        double eigen_time = run_Eigen_symm(size, 0.4);
        double gsl_time = run_GSL_symm(size, 0.4);
        //double xten_time = run_Xtensor_symm(size, 0.4);
        spdlog::info("{}, {:.2f}, {:.2f}, {:.2f}, {:.2f}", size, arma_time, blaze_time, eigen_time, gsl_time);
        return std::make_tuple(size, arma_time, blaze_time, eigen_time, gsl_time);
    };
    run_bench(bench, sym_file);
}

void asymmetric_eigen_bench() {
    std::filesystem::path asym_file = fmt::format("csv_asymm_results_{:%Y-%m-%d-%H-%M-%S}.csv",
                                                  fmt::localtime(std::chrono::system_clock::now()));

    auto bench = [](auto size) {
        double arma_time = run_Arma_asymm(size);
        double blaze_time = run_Blaze_asymm(size);
        double eigen_time = run_Eigen_asymm(size);
        double gsl_time = run_GSL_asymm(size);
        //double xten_time = run_Xtensor_asymm(size);
        spdlog::info("{}, {:.2f}, {:.2f}, {:.2f}, {:.2f}", size, arma_time, blaze_time, eigen_time, gsl_time);
        return std::make_tuple(size, arma_time, blaze_time, eigen_time, gsl_time);
    };
    run_bench(bench, asym_file);
}
