#ifndef PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#define PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#include "omp_automaton.hpp"
#include "sequential_automaton.hpp"
/**
 * @brief Namespace of the framework.
 *
 */
namespace ca
{
/**
 * @brief Interface of the CellularAutomaton
 */
class AbstractCellularAutomaton
{
  public:
    /**
     * @brief Run the simulation for a given number of steps.
     *
     * @param steps number of simulation steps to run.
     *
     */
    virtual void simulate(unsigned steps = 1) = 0;
    /**
     * @brief Get the generation of the simulation.
     *
     * @return size_t value of the generation member variable.
     */
    virtual size_t get_generation() = 0;
};

/**
 * @brief Namespace containing utility functions
 *
 */
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
{
    T **grid = new T *[nrows];
    for (size_t i{0}; i < nrows; ++i)
    {
        grid[i] = new T[ncols];
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
    for (size_t i{0}; i < rows; ++i)
    {
        delete[] grid[i];
    }
    delete[] grid;
}

} // namespace utils

} // namespace ca

#endif