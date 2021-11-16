// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include "../../include/cellular_automata.hpp"

using namespace ca;

TEST_CASE("The newGrid and deleteGrid functions work correctly")
{
    int **grid;
    size_t nrows{20};
    SECTION("newGrid")
    {
        grid = utils::newGrid<int>(nrows, nrows);
    }
    SECTION("deleteGrid")
    {
        utils::deleteGrid(grid, nrows);
    }
}