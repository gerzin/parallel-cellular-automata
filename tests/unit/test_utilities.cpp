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
        REQUIRE(grid != nullptr);
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

    auto g1 = utils::newGrid<int>(5, 5, 10);
    auto g2 = utils::newGrid<int>(5, 5, 10);

    REQUIRE(g1[0][0] == 10);
    REQUIRE(utils::areGridsEqual(g1, g2, 5, 5));
}