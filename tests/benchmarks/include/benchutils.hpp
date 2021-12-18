/**
 * @file benchutils.hpp
 * @brief This file contains utilities to speed up the benchmarks.
 * @version 0.1
 * @date 2021-11-30
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_BENCHMARK_BENCHUTILS_HPP
#define PARALLEL_CELLULAR_AUTOMATA_BENCHMARK_BENCHUTILS_HPP
#include <chrono>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

/**
 * @brief Update rule for the Conway's game of life.
 *
 * @tparam T type of the cells (int/short)
 * @param centercell central cell
 * @param tl top-left cell
 * @param t top cell
 * @param tr top-right cell
 * @param l left cell
 * @param r right cell
 * @param bl bottom-left cell
 * @param b bottom cell
 * @param br bottom-right cell
 * @return T new state of the centercell
 */
template <typename T>
T conways_game_of_life_update_function(T centercell, T tl, T t, T tr, T l, T r, T bl, T b, T br)
{
    auto alive_neighbours = tl + t + tr + l + r + bl + b + br;
    if (centercell)
    {
        // rules for alive cell
        if (alive_neighbours == 2 || alive_neighbours == 3)
            return 1;
        else
            return 0;
    }
    else
    {
        // rules for dead cell
        if (alive_neighbours == 3)
            return 1;
        else
            return 0;
    }
}

/**
 * @brief Positions of the 1s in the glider gun.
 * @code
 * 	........................O...........
 *	......................O.O...........
 *  ............OO......OO............OO
 *  ...........O...O....OO............OO
 *  OO........O.....O...OO..............
 *  OO........O...O.OO....O.O...........
 *  ..........O.....O.......O...........
 *  ...........O...O....................
 *  ............OO......................
 * @endcode
 */
const std::vector<std::pair<short, short>> glider_cells = {
    {0, 24}, {1, 22}, {1, 24}, {2, 12}, {2, 13}, {2, 20}, {2, 21}, {2, 34}, {2, 35}, {3, 11}, {3, 15}, {3, 20},
    {3, 21}, {3, 34}, {3, 35}, {4, 0},  {4, 1},  {4, 10}, {4, 16}, {4, 20}, {4, 21}, {5, 0},  {5, 1},  {5, 10},
    {5, 14}, {5, 16}, {5, 17}, {5, 22}, {5, 24}, {6, 10}, {6, 16}, {6, 24}, {7, 11}, {7, 15}, {8, 12}, {8, 13}};

/**
 * @brief Inserts a glider gun into a grid.
 *
 * This can be useful to check the correck functioning of an automaton using the Game of Life rules.
 * @see conways_game_of_life_update_function.
 * @tparam type of the cells (int/short)
 * @param grid grid.
 * @param nrows number of rows.
 * @param ncols number of columns.
 * @pre nrows >= 9   (possibly >>)
 * @pre ncols >= 35  (possibly >>)
 */
template <typename T>
void fill_grid_with_gosper_glider_gun(T **grid, size_t nrows, size_t ncols)
{
    if (nrows < 9 || ncols < 35)
    {
        std::cerr << "The grid is too small to fit the glider." << std::endl;
        std::cerr << "Grid was (" << nrows << ", " << ncols << "). Needed at least (9,35)" << std::endl;
        throw std::invalid_argument("Grid too small");
    }

    for (const auto &tuple : glider_cells)
    {
        auto [row, col] = tuple;
        grid[row][col] = 1;
    }
}

/**
 * @brief Simulates work by making a thread sleep for a number of milliseconds.
 *
 * @param millis milliseconds to sleep.
 */
void simulate_work(unsigned millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

#endif
