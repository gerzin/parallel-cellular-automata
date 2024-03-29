#include <benchmark/benchmark.h>
#include <benchutils.hpp>
#include <cellular_automata.hpp>
#include <iostream>

static void BM_GridCreationAndDeletionUInt(benchmark::State &state)
{
    size_t n = state.range(0);
    for (auto _ : state)
    {
        // this code gets timed
        auto grid = ca::Grid<int>(n, n);
    }
}

static void BM_GridCreationAndDeletionUInt8(benchmark::State &state)
{
    size_t n = state.range(0);
    for (auto _ : state)
    {
        // this code gets timed
        auto grid = ca::Grid<uint8_t>(n, n);
    }
}

static void BM_SimulationStep(benchmark::State &state)
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

static void BM_UpdateFunction(benchmark::State &state)
{

    auto update_fn = conways_game_of_life_update_function<int>;
    int y = 0;
    for (auto _ : state)
    {
        // auto val __attribute__((unused)) = update_fn(1, 0, 1, 0, 1, 0, 1, 0, 1);
        benchmark::DoNotOptimize(y = update_fn(1, 0, 1, 0, 1, 0, 1, 0, 1));
    }
}

BENCHMARK(BM_GridCreationAndDeletionUInt)->Arg(1e1)->Arg(1e2)->Arg(1e3)->Arg(1e4)->Arg(2 * 1e4);

BENCHMARK(BM_GridCreationAndDeletionUInt8)->Arg(1e1)->Arg(1e2)->Arg(1e3)->Arg(1e4)->Arg(2 * 1e4);

BENCHMARK(BM_UpdateFunction);

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