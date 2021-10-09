#ifndef PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#define PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#include <functional>
#include <iostream>
#include <tuple>

namespace ca
{
namespace seq
{
/**
 * @brief Sequential Cellular Automaton.
 *
 * This class implements a toroidal Cellular Automaton.
 *
 * @tparam T type of the cells.
 */
template <typename T>
class CellularAutomaton
{
  public:
    /**
     * @brief Run the simulation for a given number of steps.
     *
     * @param steps number of simulation steps to run (default 1).
     * @post grid contains the result of the simulation and generation = generation + steps.
     */
    virtual void simulate(unsigned steps = 1)
    {
        ;
    }

  protected:
    /**
     * @brief Grid of the C.A.
     */
    T[][] grid;

    /**
     * @brief Number of columns of the grid.
     */
    const size_t columns;
    /**
     * @brief Number of rows of the grid.
     */
    const size_t rows;
    /**
     * @brief Current generation of the grid.
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
     */
    virtual std::tuple<T, T, T, T, T, T, T, T> get_neighborhood(int row, int col) const
    {
        T top_left, top, top_right, left, right, bottom_left, bottom, bottom_right;
        top_left = grid[(row - 1 + rows) % rows][(col - 1 + ncols) % ncols];
        top = grid[(row - 1 + nrows) % nrows][col];
        top_right = grid[(row - 1 + nrows) % nrows][(col + 1) % ncols];
        left = grid[row][(col - 1 + ncols) % ncols];
        right = grid[row][(col + 1) % ncols];
        bottom_left = grid[(row + 1) % nrows][(col - 1 + ncols) % ncols];
        bottom = grid[(row + 1) % nrows][col];
        bottom_right = grid[(row + 1) % nrows][(col + 1) % ncols];
        return std::make_tuple(top_left, top, top_right, left, right, bottom_left, bottom, bottom_right);
    };
};
} // namespace seq
} // namespace ca

#endif