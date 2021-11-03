#include "../../include/cellular_automata.hpp"
#include <memory>
int main()

{
    const size_t rows = 2;
    const size_t columns = 2;
    int **grid = new int *[rows];
    for (size_t i = 0; i < rows; ++i)
    {
        grid[i] = new int[columns];
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            grid[i][j] = i + j;
        }
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) { return c + 1; };
#pragma GCC diagnostic pop
    auto ca_int = ca::seq::CellularAutomaton<int>(grid, rows, columns, update_fn);
    ca_int.simulate(1);
    std::cout << ca_int;
    ca_int.simulate(2);
    std::cout << ca_int;

    for (size_t i = 0; i < rows; ++i)
    {
        delete[] grid[i];
    }
}