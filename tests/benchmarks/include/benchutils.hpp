/**
 * @file benchutils.hpp
 * @brief This file contains utilities to speed up the benchmarks.
 * @version 0.1
 * @date 2021-11-30
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_BENCHMARK_BENCHUTILS_HPP
#define PARALLEL_CELLULAR_AUTOMATA_BENCHMARK_BENCHUTILS_HPP

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

#endif