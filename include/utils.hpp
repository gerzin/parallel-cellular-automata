/**
 * @file utils.hpp
 * @author Gerardo Zinno (g.zinno@studenti.unipi.it)
 * @brief
 * @version 0.1
 * @date 2022-01-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_UTILS_HPP
#define PARALLEL_CELLULAR_AUTOMATA_UTILS_HPP
#include <algorithm>
#include <iostream>

namespace ca
{
namespace utils
{

/**
 * @brief Allocate a new grid to be used by the automaton.
 *
 * The grid is implemented as a double-pointer matrix.
 *
 * @tparam T type of the cells
 * @param nrows number of rows of the grid.
 * @param ncols number of columns of the grid
 * @return T** pointer to the matrix.
 */
template <typename T>
T **newGrid(size_t nrows, const size_t ncols)
{ // TODO: if the deallocation of individual chunck by threads isn't faster, just allocate one big chunk and then assign
  // pointers to rows
    T **grid = new T *[nrows];
    for (size_t i{0}; i < nrows; ++i)
    {
        grid[i] = new T[ncols];
    }
    return grid;
}

/**
 * @brief Allocate a new grid to be used by the automaton and initialise each cell with an initial value.
 *
 * The grid is implemented as a double-pointer matrix.
 *
 * @tparam T type of the cells
 * @param nrows number of rows of the grid.
 * @param ncols number of columns of the grid.
 * @param initial value used to initialise the cells.
 * @return T** pointer to the matrix.
 */
template <typename T>
T **newGrid(size_t nrows, const size_t ncols, T initial)
{
    T **grid = newGrid<T>(nrows, ncols);

    for (size_t i = 0; i < nrows; ++i)
    {
        for (size_t j = 0; j < ncols; ++j)
        {
            grid[i][j] = initial;
        }
    }

    return grid;
}

/**
 * @brief Free a grid allocated with newGrid.
 * @see newGrid
 * @tparam T type of the grid cells.
 * @param grid the greed to free.
 * @param nrows number of rows of the grid.
 */
template <typename T>
void deleteGrid(T **grid, const size_t nrows)
{
    for (size_t i{0}; i < nrows; ++i)
    {
        delete[] grid[i];
    }
    delete[] grid;
}

/**
 * @brief Compare two grids
 *
 * @tparam T
 * @param g1 first grid.
 * @param g2 second gris.
 * @param ncols number of columns.
 * @param nrows number of rows.
 * @return true
 * @return false
 */
template <typename T>
bool areGridsEqual(T **g1, T **g2, size_t nrows, size_t ncols)
{
    for (size_t i{0}; i < nrows; ++i)
    {
        for (size_t j{0}; j < ncols; ++j)
        {
            if (g1[i][j] != g2[i][j])
                return false;
        }
    }
    return true;
}

template <typename T>
void printGrid(T **g, size_t nrows, size_t ncols)
{
    for (size_t i{0}; i < nrows; ++i)
    {
        for (size_t j{0}; j < ncols; ++j)
        {
            std::cout << g[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

} // namespace utils
} // namespace ca

#endif
