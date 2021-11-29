#include "../../include/cellular_automata.hpp"
#include <iostream>
using namespace ca;
using namespace std;

int main()
{
    int **grid = nullptr;
    size_t nrows{20};

    {
        grid = utils::newGrid<int>(nrows, nrows);
    }

    for (size_t i = 0; i < nrows; ++i)
    {
        for (size_t j = 0; j < nrows; ++j)
        {
            grid[i][j] = 9;
        }
    }

    for (size_t i = 0; i < nrows; ++i)
    {
        for (size_t j = 0; j < nrows; ++j)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }

    utils::deleteGrid<int>(grid, nrows);
}