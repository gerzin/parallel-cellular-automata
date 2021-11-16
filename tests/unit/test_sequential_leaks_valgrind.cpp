/**
 * @file test_sequential_leaks_valgrind.cpp
 * @brief This program will be run under valgrind in order to test for memory leaks in the
 *        sequential implementation of the automaton.
 * @version 0.1
 * @date 2021-11-14
 */
#include "../../include/cellular_automata.hpp"
#include <memory>

using namespace ca;

int main()
{
    {
        const size_t rows = 100;
        const size_t columns = 100;
        double **grid = new double *[rows];
        for (size_t i{0}; i < rows; ++i)
        {
            grid[i] = new double[columns];
        }
        auto update_fn = [](double x, int a, int b, int c, int d, int e, int f, int g, int h) {
            return x + a + b + c + d + e + f + g + h;
        };

        auto c = seq::CellularAutomaton<double>(grid, rows, columns, update_fn);
        c.simulate(0);
        c.simulate(1);

        for (size_t i{0}; i < rows; ++i)
        {
            delete[] grid[i];
        }
        delete[] grid;
    }

    {
        using str = std::string;
        size_t nrows = 20;
        auto grid = ca::utils::newGrid<str>(nrows, nrows);
        auto update_fn = [](str, str, str, str, str, str, str, str, str) { return str(""); };
        auto c = seq::CellularAutomaton<str>(grid, nrows, nrows, update_fn);
        ca::utils::deleteGrid(grid, nrows);
    }

    return EXIT_SUCCESS;
}
