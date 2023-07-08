/**
 * @file sequential_automaton.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This file contains the definition and implementation of the sequential version of the Cellular Automaton.
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_SEQUENTIAL_AUTOMATON_HPP
#define PARALLEL_CELLULAR_AUTOMATA_SEQUENTIAL_AUTOMATON_HPP
#include <functional>
#include <iostream>
#include <tuple>

#ifndef PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#include "cellular_automata.hpp"
#endif
#include "grid.hpp"

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
     * @param update_function Function used to update the state of the grid.
     *
     */
    CellularAutomaton(Grid<T> &grid, std::function<T(T, T, T, T, T, T, T, T, T)> update_function)
        : grid{grid}, generation(0), update_function(update_function){};

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
        update_function = std::move(other.update_function);
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
        auto new_grid = ca::Grid<int>::newWithSameSize(grid);

        // compute state and put values on the new grid.
        while (steps > 0)
        {
            for (size_t r{0}; r < grid.rows(); ++r)
            {
                for (size_t c{0}; c < grid.columns(); ++c)
                {
                    auto cell = std::make_tuple(grid(r, c));
                    new_grid(r, c) = std::apply(update_function, std::tuple_cat(cell, get_neighborhood(r, c)));
                }
            }
            // swap grids so grid contains the final value.
            grid.swap(new_grid);

            ++generation;
            --steps;
        }
    }

    /**
     * @brief Get the generation of the simulation.
     *
     * @return size_t value of the generation member variable.
     */
    virtual size_t get_generation() const
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

        return os << ca.grid;
    }

  protected:
    /**
     * @brief Grid of the C.A.
     *
     */
    Grid<T> &grid;

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
        unsigned rows = grid.rows();
        unsigned columns = grid.columns();
        T top_left, top, top_right, left, right, bottom_left, bottom, bottom_right;
        top_left = grid((row - 1 + rows) % rows, (col - 1 + columns) % columns);
        top = grid((row - 1 + rows) % rows, col);
        top_right = grid((row - 1 + rows) % rows, (col + 1) % columns);
        left = grid(row, (col - 1 + columns) % columns);
        right = grid(row, (col + 1) % columns);
        bottom_left = grid((row + 1) % rows, (col - 1 + columns) % columns);
        bottom = grid((row + 1) % rows, col);
        bottom_right = grid((row + 1) % rows, (col + 1) % columns);
        return std::make_tuple(top_left, top, top_right, left, right, bottom_left, bottom, bottom_right);
    };
};
} // namespace seq
} // namespace ca

#endif