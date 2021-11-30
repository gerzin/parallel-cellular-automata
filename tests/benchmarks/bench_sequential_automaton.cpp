#include <benchmark/benchmark.h>
#include <cellular_automata.hpp>

static void BM_GridCreationAndDeletion(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto grid = ca::utils::newGrid<int>(10, 10, 0);
        ca::utils::deleteGrid<int>(grid, 10);
    }
}

BENCHMARK(BM_GridCreationAndDeletion);

BENCHMARK_MAIN();