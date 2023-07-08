/**
 * @file cuda_automaton.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This file contains the definition and implementation of the Cellular Automaton parallelized using CUDA.
 * @version 0.1
 * @date 2021-12-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_CUDA_AUTOMATON_HPP
#define PARALLEL_CELLULAR_AUTOMATA_CUDA_AUTOMATON_HPP
#include <cstddef>
#include <functional>
#include <grid.hpp>
#include <iostream>
#include <tuple>

namespace ca
{
/**
 * @brief Namespace containing the implementation of the parallelized version using CUDA.
 *
 */
namespace cuda
{
/**
 * @brief Parallel Cellular Automaton using CUDA.
 *
 * This class implements a toroidal Cellular Automaton parallelized using CUDA.
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
     * @param workers Number of threads. If 0 the default value is chosen by CUDA.
     *
     */
    CellularAutomaton(ca::Grid<T> &grid, std::function<T(T, T, T, T, T, T, T, T, T)> update_function,
                      unsigned workers = 0)
        : grid(grid), generation(0), update_function(update_function)
    {
        nw = (workers) ? workers : std::thread::hardware_concurrency();
    };

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
        Grid<T> new_grid = ca::Grid<T>::newWithSameSize(grid);

        // compute state and put values on the new grid.
        while (steps > 0)
        {
#pragma omp parallel for collapse(2) num_threads(nw)
            for (size_t r = 0; r < grid.rows(); ++r)
            {
                for (size_t c = 0; c < grid.columns(); ++c)
                {
                    auto cell = std::make_tuple(grid(r, c));
                    new_grid(r, c) = std::apply(update_function, std::tuple_cat(cell, get_neighborhood(r, c)));
                }
            }
            // swap grids so grid contains the updated value.
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
     * @brief Number of worker threads.
     *
     */
    unsigned nw;

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
} // namespace cuda
} // namespace ca

#endif