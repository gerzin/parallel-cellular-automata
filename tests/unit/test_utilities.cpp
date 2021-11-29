// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include "../../include/cellular_automata.hpp"

using namespace ca;
using namespace std;

TEST_CASE("The newGrid and deleteGrid functions work correctly")
{
    int **grid = nullptr;
    size_t nrows{20};
    SECTION("newGrid")
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

    SECTION("deleteGrid")
    {
        utils::deleteGrid<int>(grid, nrows);
    }
}