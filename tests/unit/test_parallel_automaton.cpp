/*
 * I test the private member functions of the seq::CellularAutomaton by copypasting
 * here the code of the function and defining the right global variables.
 * It's ugly but still less ugly than something like #define private public, #define protected public
 * or modifying the original code by inserting in it a friend class which potentially could alter
 * the behaviour of the code.
 */
#include "../../include/cellular_automata.hpp"
// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include <memory>

TEST_CASE("The parallel cellular automaton is created.")
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
    auto automaton = ca::par::CellularAutomaton<int>(grid, update_fn, 4);
    std::cout << automaton << std::endl;

    automaton.simulate(0);
    automaton.simulate(1);
    automaton.simulate(0);
    automaton.simulate(2);

    REQUIRE(automaton.get_generation() == 3);
}