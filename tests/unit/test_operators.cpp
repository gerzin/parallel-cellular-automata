#include "../../include/cellular_automata.hpp"
// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include <memory>

TEST_CASE("ca::seq::CellularAutomaton's operator<< works correctly")
{
    // creating 
    const size_t rows = 2;
    const size_t columns = 2;
    auto grid = std::make_unique<int *[]>(rows);
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
    auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) { return c; };
#pragma GCC diagnostic pop

    auto ca_int = ca::seq::CellularAutomaton<int>(grid.get(), rows, columns, update_fn);
    std::ostringstream oss;
    oss << ca_int;
    REQUIRE(std::string(oss.str()) == std::string("0 1 \n1 2 \n"));

    if (grid)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            delete[] grid[i];
        }
    }
}