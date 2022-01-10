#include "include/benchutils.hpp"
#include <benchmark/benchmark.h>
#include <cellular_automata.hpp>
#include <iostream>
using namespace ca;

static void BM_SimulationStep(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    auto grid = Grid<int>(nrows, ncols);
    auto cellaut = omp::CellularAutomaton<int>(grid, conways_game_of_life_update_function<int>);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

BENCHMARK(BM_SimulationStep)
    // 1 simulation step
    ->Args({static_cast<size_t>(1e1), 1})
    ->Args({static_cast<size_t>(1e2), 1})
    ->Args({static_cast<size_t>(1e3), 1})
    ->Args({static_cast<size_t>(1e4), 1})
    // 5 simulation steps
    ->Args({static_cast<size_t>(1e1), 5})
    ->Args({static_cast<size_t>(1e2), 5})
    ->Args({static_cast<size_t>(1e3), 5})
    ->Args({static_cast<size_t>(1e4), 5});

BENCHMARK_MAIN();