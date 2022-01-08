#include "../../include/cellular_automata.hpp"
#include "../../include/utimer.hpp"
#include "../benchmarks/include/benchutils.hpp"

#define LOG(args)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cerr << args << std::endl;                                                                                \
    } while (0);

#define LOGA(args)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cerr << args << std::endl;                                                                                \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0);

int main()
{
    // FIXME: il segfault è causato dallo swap. Creare grid wrapper
    const size_t ROWS = 60, COLUMNS = 60;
    /*auto simple_update_function = [](int c, int a_, int b_, int c_, int d_, int e_, int f_, int g_, int h_) {
        (void)a_;
        (void)b_;
        (void)c_;
        (void)d_;
        (void)e_;
        (void)f_;
        (void)g_;
        (void)h_;
        (void)c;
        return c + 1;
    };
    */
    auto par_grid = ca::utils::newGrid<int>(ROWS, COLUMNS, 0);
    auto par_automaton =
        ca::par::CellularAutomaton<int>(par_grid, ROWS, COLUMNS, conways_game_of_life_update_function<int>, 4);
    fill_grid_with_gosper_glider_gun<int>(par_grid, ROWS, COLUMNS);

    auto seq_grid = ca::utils::newGrid<int>(ROWS, COLUMNS, 0);
    auto seq_automaton =
        ca::seq::CellularAutomaton<int>(seq_grid, ROWS, COLUMNS, conways_game_of_life_update_function<int>);
    fill_grid_with_gosper_glider_gun<int>(seq_grid, ROWS, COLUMNS);

    seq_grid[0][0] = par_grid[0][0] = 3;

    if (!ca::utils::areGridsEqual<int>(par_grid, seq_grid, ROWS, COLUMNS))
    {
        LOGA("The grids aren't equal before starting");
    }
    {
        UTimer t("20 sequential simulations");
        {
            seq_automaton.simulate(9);
        }
    }
    {
        UTimer t("20 parallel simulations");
        {
            par_automaton.simulate(9);
        }
    }
    /* if (!ca::utils::areGridsEqual<int>(par_grid, seq_grid, ROWS, COLUMNS))
     {
         LOGA("The grids aren't equal after simulations");
     }
 */
    std::cout << seq_automaton << std::endl << par_automaton << std::endl;
    std::cout << seq_grid[0][0] << " " << par_grid[0][0] << std::endl;
    ca::utils::deleteGrid<int>(par_grid, ROWS);
    ca::utils::deleteGrid<int>(seq_grid, ROWS);
}