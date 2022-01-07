#include "../../include/cellular_automata.hpp"
#include "../../include/utimer.hpp"
#include "../benchmarks/include/benchutils.hpp"

#define LOG(args)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cerr << args << std::endl                                                                                 \
    } while (0);

#define LOGA(args)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cerr << args << std::endl;                                                                                \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0);

int main()
{
    const size_t ROWS = 100, COLUMNS = 100;

    auto par_grid = ca::utils::newGrid<int>(ROWS, COLUMNS, 0);
    auto par_automaton =
        ca::par::CellularAutomaton<int>(par_grid, ROWS, COLUMNS, conways_game_of_life_update_function<int>);
    fill_grid_with_gosper_glider_gun<int>(par_grid, ROWS, COLUMNS);

    auto seq_grid = ca::utils::newGrid<int>(ROWS, COLUMNS, 0);
    auto seq_automaton =
        ca::seq::CellularAutomaton<int>(par_grid, ROWS, COLUMNS, conways_game_of_life_update_function<int>);
    fill_grid_with_gosper_glider_gun<int>(seq_grid, ROWS, COLUMNS);

    if (!ca::utils::areGridsEqual<int>(par_grid, seq_grid, ROWS, COLUMNS))
    {
        LOGA("The grids aren't equal before starting");
    }
    {
        UTimer t("20 sequential simulations");
        {
            seq_automaton.simulate(20);
        }
    }
    {
        UTimer t("20 parallel simulations");
        {
            par_automaton.simulate(20);
        }
    }
    if (!ca::utils::areGridsEqual<int>(par_grid, seq_grid, ROWS, COLUMNS))
    {
        LOGA("The grids aren't equal after simulations");
    }
    ca::utils::deleteGrid<int>(par_grid, ROWS);
    ca::utils::deleteGrid<int>(seq_grid, ROWS);
}