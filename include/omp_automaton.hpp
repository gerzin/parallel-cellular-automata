#ifndef PARALLEL_CELLULAR_AUTOMATA_OMPL_AUTOMATON_HPP
#define PARALLEL_CELLULAR_AUTOMATA_OMPL_AUTOMATON_HPP
#include <cstddef>
#include <functional>
#include <iostream>
#include <tuple>

/*#ifndef _OPENMP
// use message instead of warning because this file may be included but not used, and
// want to avoid problems with eventual -Werror
#pragma message("You are including this file: " __FILE__ " which uses OpenMP but _OPENMP is not defined."              \
                " This may be totally fine if you're including cellular_automata.hpp into a project"                   \
                "and not call functions from the ca::omp namespace.")
#endif
*/

namespace ca
{
/**
 * @brief Namespace containing the implementation of the parallelized version using OpenMP.
 *
 */
namespace omp
{
/**
 * @brief Parallel Cellular Automaton using OpenMP.
 *
 * This class implements a toroidal Cellular Automaton parallelized using OpenMP.
 *
 * @tparam T type of the cells.
 *
 */
template <typename T>
class CellularAutomaton
{
  public:
    /**
     * @brief Construct a new Cellular Automaton object.
     *
     * @param grid Grid of the simulation.
     * @param rows Number of rows of the grid.
     * @param columns Number of columns of the grid.
     * @param update_function Function used to update the state of the grid.
     *
     */
    CellularAutomaton(T **grid, const size_t rows, const size_t columns,
                      std::function<T(T, T, T, T, T, T, T, T, T)> update_function)
        : grid{grid}, rows{rows}, columns{columns}, generation(0), update_function(update_function){};
    /**
     * @brief Construct a new Cellular Automaton object from another one using move semantic.
     * @note The old object will be left in a valid but unspecified state.
     * @param other CellularAutomaton to move.
     *
     */
    CellularAutomaton(CellularAutomaton &&other)
    {
        // move what's movable and copying numeric types.
        grid = std::move(other.grid);
        rows = other.rows;
        columns = other.columns;
        generation = other.generation;
        update_function = other.update_function;
        // set the old object in a valid state
        other.grid = nullptr;
        other.columns = 0;
        other.rows = 0;
        other.generation = 0;
    }

    /**
     * @brief Deleted copy constructor.
     *
     * @note The copy constructor has been deleted since the class uses a pointer
     *       to a user defined grid and the freeing of the grid is up to them.
     *
     */
    CellularAutomaton(const CellularAutomaton &other) = delete;

    /**
     * @brief Run the simulation for a given number of steps.
     *
     * @param steps number of simulation steps to run (default 1).
     * @post grid contains the result of the simulation and generation = generation + steps.
     *
     */
    virtual void simulate(unsigned steps = 1)
    {
        if (steps == 0)
            return;
        // allocate new grid
        T **new_grid = new T *[rows];
#pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
        {
            new_grid[i] = new T[columns];
        }
        // compute state and put values on the new grid.
        while (steps > 0)
        {
#pragma omp parallel for collapse(2)
            for (size_t r = 0; r < rows; ++r)
            {
                for (size_t c = 0; c < columns; ++c)
                {
                    auto cell = std::make_tuple(grid[r][c]);
                    new_grid[r][c] = std::apply(update_function, std::tuple_cat(cell, get_neighborhood(r, c)));
                }
            }
            // swap grids so grid contains the final value.
            T *tmp = *grid;
            *grid = *new_grid;
            *new_grid = tmp;

            ++generation;
            --steps;
        }
// free the memory of the new grid
#pragma omp parallel for
        for (size_t i = 0; i < rows; ++i)
        {
            delete[] new_grid[i];
        }
        delete[] new_grid;
    }

    /**
     * @brief Get the generation of the simulation.
     *
     * @return size_t value of the generation member variable.
     */
    size_t get_generation() const
    {
        return generation;
    }
    /**
     * @brief Overload of the << operator.
     *
     * Print the grid on a output stream.
     *
     * @param os output stream
     * @param ca CellularAutomaton
     * @return std::ostream& reference to the output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const CellularAutomaton &ca)
    {
        for (size_t i = 0; i < ca.rows; ++i)
        {
            for (size_t j = 0; j < ca.columns; ++j)
            {
                os << ca.grid[i][j] << " ";
            }

            os << std::endl;
        }

        return os;
    }

  protected:
    /**
     * @brief Grid of the C.A.
     *
     */
    T **grid;

    /**
     * @brief Number of rows of the grid.
     *
     */
    const size_t rows;

    /**
     * @brief Number of columns of the grid.
     *
     */
    const size_t columns;

    /**
     * @brief Current generation of the grid.
     *
     */
    size_t generation;
    /**
     * @brief Function used to compute the next state of the cell.
     *
     * The function takes nine arguments. It will be called by passing the
     * central cell as first argument and its eight neighbors as the remaining
     * arguments. Their order is the one returned by the get_neighborhood
     * function.
     * @see get_neighborhood
     *
     */
    std::function<T(T, T, T, T, T, T, T, T, T)> update_function;
    /**
     * @brief Get the neighborhood of a cell.
     * @code
     *    +----+-----------+----+
     *    | TL |     T     | TR |
     *    +----+-----------+----+
     *    | L  | (row,col) | R  |
     *    +----+-----------+----+
     *    | BL |     B     | BR |
     *    +----+-----------+----+
     * @endcode
     * @pre 0 <= row < rows.
     * @pre 0 <= col < columns.
     * @param row row of the center cell.
     * @param col column of the center cell.
     * @return std::tuple<T, T, T, T, T, T, T, T> containing <TL, T, TR, L, R, BL, B, BR>
     *
     */
    virtual std::tuple<T, T, T, T, T, T, T, T> get_neighborhood(int row, int col) const
    {
        T top_left, top, top_right, left, right, bottom_left, bottom, bottom_right;
        top_left = grid[(row - 1 + rows) % rows][(col - 1 + columns) % columns];
        top = grid[(row - 1 + rows) % rows][col];
        top_right = grid[(row - 1 + rows) % rows][(col + 1) % columns];
        left = grid[row][(col - 1 + columns) % columns];
        right = grid[row][(col + 1) % columns];
        bottom_left = grid[(row + 1) % rows][(col - 1 + columns) % columns];
        bottom = grid[(row + 1) % rows][col];
        bottom_right = grid[(row + 1) % rows][(col + 1) % columns];
        return std::make_tuple(top_left, top, top_right, left, right, bottom_left, bottom, bottom_right);
    };
};

/**
 * @brief Utilities using OpenMP
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
 *
 * @note same as ca::newGrid but uses OpenMP
 */
template <typename T>
T **newGrid(size_t nrows, const size_t ncols)
{
    T **grid = new T *[nrows];
#pragma omp parallel for
    for (size_t i = 0; i < nrows; ++i)
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
 * @note same as ca::newGrid but uses OpenMP.
 */
template <typename T>
T **newGrid(size_t nrows, const size_t ncols, T initial)
{
    T **grid = newGrid<T>(nrows, ncols);

#pragma omp parallel for collapse(2)
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
 * @note same as ca::deleteGrid but uses OpenMP
 */
template <typename T>
void deleteGrid(T **grid, const size_t nrows)
{
#pragma omp parallel for
    for (size_t i = 0; i < nrows; ++i)
    {
        delete[] grid[i];
    }
    delete[] grid;
}
} // namespace utils

} // namespace omp
} // namespace ca

#endif