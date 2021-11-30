#include <benchmark/benchmark.h>
#include <cellular_automata.hpp>

static void BM_GridCreation(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto grid = ca::utils::newGrid<int>(10, 10, 0);
        ca::utils::deleteGrid(grid, 10);
    }
}

BENCHMARK(BM_GridCreation);

BENCHMARK_MAIN();