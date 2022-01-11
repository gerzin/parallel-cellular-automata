// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include "../../include/cellular_automata.hpp"

TEST_CASE("The fastflow cellular automaton is created.")
{
    const auto rows = 5;
    const auto columns = 5;
    auto grid = ca::Grid<int>(rows, columns);
    for (size_t i = 0; i < static_cast<int>(rows); ++i)
    {
        for (size_t j = 0; j < static_cast<int>(columns); ++j)
        {
            grid(i, j) = i + j;
        }
    }

    auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) -> int {
        return c + tl + t + tr + l + r + bl + b + br + 1;
    };
    auto automaton = ca::ffl::CellularAutomaton<int>(grid, update_fn, 4);

    automaton.simulate(0);
    automaton.simulate(1);
    automaton.simulate(0);
    automaton.simulate(2);

    REQUIRE(automaton.get_generation() == 3);
}