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
        const size_t rows = 4;
        const size_t columns = 5;
        int grid[rows][columns] = {{0, -2, 0, 0, -1}, {0, 1, 2, 3, -2}, {0, 4, 0, 5, 0}, {9, 6, 7, 8, 0}};
        auto update_fn = [](int x, int a, int b, int c, int d, int e, int f, int g, int h) {
            return x + a + b + c + d + e + f + g + h;
        };
        auto c = seq::CellularAutomaton<int>(grid, rows, columns, update_fn);
        c.simulate(5);
        c.get_generation();
    }

    {
        const size_t rows = 100;
        const size_t columns = 100;
        double **grid = new double *[rows];
        for (int i = 0; i < rows; ++i)
        {
            grid[i] = new double[columns];
        }
        auto update_fn = [](double x, int a, int b, int c, int d, int e, int f, int g, int h) {
            return x + a + b + c + d + e + f + g + h;
        };

        auto c = seq::CellularAutomaton<double>(grid, rows, columns, update_fn);
        c.simulate(0);
        c.simulate(1);

        for (int i = 0; i < rows; ++i)
        {
            delete[] grid[i];
        }
        delete[] grid;
    }

    return EXIT_SUCCESS;
}
