// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include <cellular_automata.hpp>

TEST_CASE("All the C.A. compute the same result")
{

    auto g1 = ca::Grid<int>(10, 10);
    auto g2 = ca::Grid<int>(10, 10);
    auto g3 = ca::Grid<int>(10, 10);
    auto g4 = ca::Grid<int>(10, 10);

    for (int i = 0; i < static_cast<int>(std::min(g1.rows(), g1.columns())); ++i)
    {
        g1(i, i) = i;
        g2(i, i) = i;
        g3(i, i) = i;
        g4(i, i) = i;
    }

    REQUIRE(g1 == g2);
    REQUIRE(g2 == g3);
    REQUIRE(g3 == g4);

    auto update = [&](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) {
        auto sum = c + tl + t + tr + l + r + l + bl + b + br;
        if (sum > 2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    };

    int nsteps = 6;
    unsigned nw = 4;

    auto seq = ca::seq::CellularAutomaton<int>(g1, update);
    auto omp = ca::omp::CellularAutomaton<int>(g2, update, nw);
    auto par = ca::par::CellularAutomaton<int>(g3, update, nw);
    auto ff = ca::ffl::CellularAutomaton<int>(g4, update, nw);

    seq.simulate(nsteps);
    omp.simulate(nsteps);
    par.simulate(nsteps);
    ff.simulate(nsteps);

    REQUIRE(g1 == g2);
    REQUIRE(g2 == g3);
    REQUIRE(g3 == g4);

    REQUIRE(seq.get_generation() == static_cast<size_t>(nsteps));
    REQUIRE(seq.get_generation() == omp.get_generation());
    REQUIRE(seq.get_generation() == ff.get_generation());
    REQUIRE(seq.get_generation() == par.get_generation());
}