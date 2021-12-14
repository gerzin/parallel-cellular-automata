#include "include/benchutils.hpp"
#include <benchmark/benchmark.h>
#include <cellular_automata.hpp>
#include <iostream>

static void BM_GridCreationAndDeletion(benchmark::State &state)
{
    size_t n = state.range(0);
    for (auto _ : state)
    {
        // this code gets timed
        auto grid = ca::omp::utils::newGrid<int>(n, n, 0);
        ca::omp::utils::deleteGrid<int>(grid, n);
    }
}

static void BM_SimulationStep(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = state.range(1);
    auto grid = ca::omp::utils::newGrid<int>(nrows, ncols, 0);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::omp::CellularAutomaton<int>(grid, nrows, ncols, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
    ca::utils::deleteGrid<int>(grid, nrows);
}

BENCHMARK(BM_GridCreationAndDeletion)->Arg(1e1)->Arg(1e2)->Arg(1e3)->Arg(1e4)->Arg(2 * 1e4);

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