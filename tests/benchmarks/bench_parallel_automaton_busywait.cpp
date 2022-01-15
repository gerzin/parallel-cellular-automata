#include <benchmark/benchmark.h>
#include <benchutils.hpp>
#include <cellular_automata.hpp>
#include <iostream>

static void BM_SimulationStepBW(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::bw::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStep(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepBW4Threads(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::bw::CellularAutomaton<int>(grid, update_fn, 4);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

BENCHMARK(BM_SimulationStep)
    // 1 simulation step
    ->Arg(static_cast<size_t>(1e2))
    ->Arg(static_cast<size_t>(1e3))
    ->Arg(static_cast<size_t>(1e4));

BENCHMARK(BM_SimulationStepBW)
    // 1 simulation step
    ->Arg(static_cast<size_t>(1e2))
    ->Arg(static_cast<size_t>(1e3))
    ->Arg(static_cast<size_t>(1e4));

BENCHMARK(BM_SimulationStepBW4Threads)
    // 1 simulation step
    ->Arg(static_cast<size_t>(1e2))
    ->Arg(static_cast<size_t>(1e3))
    ->Arg(static_cast<size_t>(1e4));

BENCHMARK_MAIN();