#ifndef PARALLEL_CELLULAR_AUTOMATA_SEQUENTIAL_AUTOMATON_HPP
#define PARALLEL_CELLULAR_AUTOMATA_SEQUENTIAL_AUTOMATON_HPP
#include <functional>
#include <iostream>
#include <tuple>

namespace ca
{
/**
 * @brief Namespace containing the implementation of the sequential version.
 *
 */
namespace seq
{
/**
 * @brief Sequential Cellular Automaton.
 *
 * This class implements a toroidal Cellular Automaton.
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
    CellularAutomaton(T **grid, size_t rows, size_t columns,
                      std::function<T(T, T, T, T, T, T, T, T, T)> update_function)
        : grid{grid}, rows{rows}, columns{columns} {};
    /**
     * @brief Destroy the Cellular Automaton object.
     *
     * If the grid is not a nullptr it deletes the memory allocated.
     *
     */
    ~CellularAutomaton() // TODO: possibly remove this destructor.
    {
        if (grid != nullptr)
        {
            for (auto r{0}; r < rows; ++r)
            {
                delete[] grid[r];
            }
            delete[] grid;
        }
    }
    /**
     * @brief Run the simulation for a given number of steps.
     *
     * @param steps number of simulation steps to run (default 1).
     * @post grid contains the result of the simulation and generation = generation + steps.
     *
     */
    virtual void simulate(unsigned steps = 1)
    {
        // allocate new grid
        T **new_grid = new T *[rows];
        for (auto i{0}; i < rows; ++i)
        {
            new_grid[0] = new T[columns];
        }
        // compute state and put values on the new grid.
        while (steps > 0)
        {
            for (auto r{0}; r < rows; ++r)
            {
                for (auto c{0}; c < columns; ++c)
                {
                    auto cell = std::make_tuple(grid[row][col]);
                    new_grid[r][c] = std::apply(update_function, std::tuple_cat(cell, get_neighborhood(row, col)));
                }
            }
            // swap grids so grid contains the final value.
            auto tmp = grid;
            grid = new_grid;
            new_grid = tmp;

            ++generation;
            --steps;
        }
    }

  protected:
    /**
     * @brief Grid of the C.A.
     *
     */
    T **grid;

    /**
     * @brief Number of columns of the grid.
     *
     */
    const size_t columns;
    /**
     * @brief Number of rows of the grid.
     *
     */
    const size_t rows;
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
     * @pre 0 <= row < rows && 0 <= col < columns.
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
} // namespace seq
} // namespace ca
#endif