//
// Created by erich on 09/04/2021.
//

#ifndef MATRIX_COMPARISON_PARAMETRES_HPP
#define MATRIX_COMPARISON_PARAMETRES_HPP
#include <array>
#include <chrono>
#include <numeric>


// Max Matrix Size
constexpr size_t max_size = 256;

// Min Matrix Size
constexpr size_t min_size = 2;

// Compile time matrix size array
template <size_t SIZE>
constexpr auto table_gen() {
    std::array<size_t, SIZE-2> temp;
    std::iota(temp.begin(), temp.end(), min_size);
    return temp;
}

constexpr auto size_table = table_gen<max_size>();

// Floating point millisecond duration
using microsecond_d = std::chrono::duration<double, std::micro>;

// Number of iteration per test
constexpr size_t num_iter = 32;
#endif //MATRIX_COMPARISON_PARAMETRES_HPP
