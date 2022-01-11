// this file runs all the benchmarks
#include <benchmark/benchmark.h>
#include <benchutils.hpp>
#include <cellular_automata.hpp>
#include <iostream>

static void BM_SimulationStepSequential(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::seq::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepParallel(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    unsigned nworkers = static_cast<unsigned>(state.range(2));
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::CellularAutomaton<int>(grid, update_fn, nworkers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepOMP(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    unsigned nworkers = static_cast<unsigned>(state.range(2));
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::omp::CellularAutomaton<int>(grid, update_fn, nworkers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepFF(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    unsigned nworkers = static_cast<unsigned>(state.range(2));
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::ffl::CellularAutomaton<int>(grid, update_fn, nworkers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

BENCHMARK(BM_SimulationStepSequential)
    // 1 simulation step
    ->Args({static_cast<size_t>(1e1), 1})
    ->Args({static_cast<size_t>(1e2), 1})
    ->Args({static_cast<size_t>(1e3), 1})
    ->Args({static_cast<size_t>(1e4), 1})
    // 5 simulation steps
    ->Args({static_cast<size_t>(1e1), 5})
    ->Args({static_cast<size_t>(1e2), 5})
    ->Args({static_cast<size_t>(1e3), 5})
    ->Args({static_cast<size_t>(1e4), 5})
    // 10 simulation steps
    ->Args({static_cast<size_t>(1e3), 10});

BENCHMARK(BM_SimulationStepParallel)
    // 1 simulation step
    ->Args({static_cast<size_t>(1e1), 1})
    ->Args({static_cast<size_t>(1e2), 1})
    ->Args({static_cast<size_t>(1e3), 1})
    ->Args({static_cast<size_t>(1e4), 1})
    // 5 simulation steps
    ->Args({static_cast<size_t>(1e1), 5})
    ->Args({static_cast<size_t>(1e2), 5})
    ->Args({static_cast<size_t>(1e3), 5})
    ->Args({static_cast<size_t>(1e4), 5})
    // 10 simulation steps
    ->Args({static_cast<size_t>(1e3), 10});
