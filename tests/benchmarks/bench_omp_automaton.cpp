#include <benchmark/benchmark.h>
#include <cellular_automata.hpp>

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

BENCHMARK(BM_GridCreationAndDeletion)->Arg(1e1)->Arg(1e2)->Arg(1e3)->Arg(1e4)->Arg(1e5);

BENCHMARK_MAIN();