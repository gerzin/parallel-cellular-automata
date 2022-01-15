/**
 * @file parallel_automaton_busyw.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This file implements the parallel cellular automaton using the busy wait function of the barrier.
 *        This implementation has shown experimentally to have worse performances therfore should not be used.
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_PARALLEL_AUTOMATON_BW_HPP
#define PARALLEL_CELLULAR_AUTOMATA_PARALLEL_AUTOMATON_BW_HPP

#ifndef PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#include "cellular_automata.hpp"
#endif
#include <barrier.hpp>
#include <grid.hpp>
#include <stdexcept>
#include <thread>
#include <threadpool.hpp>
#include <vector>

using namespace std;

namespace ca
{
/**
 * @brief Namespace containing the implementation of the parallel version of the cellular automaton.
 *
 */
namespace par
{
/**
 * @brief Namespace containing the implementation of the parallel version in which the wotkers busywait for others.
 *
 */
namespace bw

{
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
     * @param nw Number of workers (0 == hadrwareconcurrency) (default 0).
     *
     */
    CellularAutomaton(ca::Grid<T> &grid, std::function<T(T, T, T, T, T, T, T, T, T)> update_function,
                      unsigned workers = 0)
        : grid{grid}, generation(0), update_function(update_function), pool(workers)
    {

        this->nw = pool.get_number_workers();
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
        generation = other.generation;
        update_function = other.update_function;
        nw = other.nw;
        pool = std::move(other.pool);
        // set the old object in a valid state
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
        Grid<T> new_grid = Grid<T>::newWithSameSize(grid);

        ca::Barrier sync_point(nw);

        auto step_advancement_fun = [&]() {
            grid.swap(new_grid);
            ++generation;
            --steps;
        };

        // function to be run by each thread
        auto work = [&, this](unsigned start, unsigned end) {
            while (steps > 0)
            {
                for (unsigned r{start}; r < end; ++r)
                {
                    for (unsigned c{0}; c < grid.columns(); ++c)
                    {
                        auto cell = std::make_tuple(grid(r, c));
                        new_grid(r, c) =
                            std::apply(this->update_function, std::tuple_cat(cell, get_neighborhood(r, c)));
                    }
                }
                sync_point.busy_wait(
                    step_advancement_fun); // this gets executed only by the last thread to reach the barrier
            }
        };

        std::vector<std::future<void>> results; // handles for waiting the threads
        unsigned delta{static_cast<unsigned>(grid.rows()) / nw};

        for (unsigned i{0}; i < nw; i++) // split the grid
        {
            unsigned start = i * delta;
            unsigned end = (i != (nw - 1) ? (i + 1) * delta : grid.rows());

            results.push_back(pool.submit(work, start, end));
        }

        for (auto &r : results)
        {
            r.wait();
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
    /**
     * @brief Number of worker threads.
     *
     */
    unsigned nw;

    /**
     * @brief The threadpool that will run the tasks.
     *
     */
    Threadpool pool;
};
} // namespace bw
} // namespace par
} // namespace ca
#endif