// this file runs all the benchmarks
#include <benchmark/benchmark.h>
#include <benchutils.hpp>
#include <cellular_automata.hpp>
#include <iostream>

// run 10 iterations on multiple grid sizes using the default number of workers

static void BM_SimulationStepSequential(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
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
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepParallelBW(benchmark::State &state)
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

static void BM_SimulationStepFF(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::ffl::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_SimulationStepOMP(benchmark::State &state)
{
    size_t nrows = state.range(0);
    size_t ncols = nrows;
    auto nsteps = 10;
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::omp::CellularAutomaton<int>(grid, update_fn);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

BENCHMARK(BM_SimulationStepSequential)->Arg(1e2)->Arg(1e3)->Arg(1e4);

BENCHMARK(BM_SimulationStepParallel)->Arg(1e2)->Arg(1e3)->Arg(1e4);

BENCHMARK(BM_SimulationStepParallelBW)->Arg(1e2)->Arg(1e3)->Arg(1e4);

BENCHMARK(BM_SimulationStepFF)->Arg(1e2)->Arg(1e3)->Arg(1e4);

BENCHMARK(BM_SimulationStepOMP)->Arg(1e2)->Arg(1e3)->Arg(1e4);

// run 10 iterations on a medium sized grid varying the number of workers

static void BM_VaryingWorkersGrid1000Parallel(benchmark::State &state)
{
    size_t nrows = 1000;
    size_t ncols = nrows;
    auto nsteps = 10;
    unsigned workers = state.range(0);
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::CellularAutomaton<int>(grid, update_fn, workers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_VaryingWorkersGrid1000ParallelBW(benchmark::State &state)
{
    size_t nrows = 1000;
    size_t ncols = nrows;
    auto nsteps = 10;
    unsigned workers = state.range(0);
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::par::CellularAutomaton<int>(grid, update_fn, workers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_VaryingWorkersGrid1000FF(benchmark::State &state)
{
    size_t nrows = 1000;
    size_t ncols = nrows;
    auto nsteps = 10;
    unsigned workers = state.range(0);
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::ffl::CellularAutomaton<int>(grid, update_fn, workers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

static void BM_VaryingWorkersGrid1000OMP(benchmark::State &state)
{
    size_t nrows = 1000;
    size_t ncols = nrows;
    auto nsteps = 10;
    unsigned workers = state.range(0);
    auto grid = ca::Grid<int>(nrows, ncols);
    auto update_fn = conways_game_of_life_update_function<int>;
    auto cellaut = ca::omp::CellularAutomaton<int>(grid, update_fn, workers);
    for (auto _ : state)
    {
        cellaut.simulate(nsteps);
    }
}

BENCHMARK(BM_VaryingWorkersGrid1000Parallel)->DenseRange(0, 17, 1);

BENCHMARK(BM_VaryingWorkersGrid1000ParallelBW)->DenseRange(0, 17, 1);

BENCHMARK(BM_VaryingWorkersGrid1000FF)->DenseRange(0, 17, 1);

BENCHMARK(BM_VaryingWorkersGrid1000OMP)->DenseRange(0, 17, 1);

BENCHMARK_MAIN();