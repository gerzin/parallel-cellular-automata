#include "../../include/cellular_automata.hpp"
#include "../benchmarks/include/benchutils.hpp"
int main()
{
    auto grid = ca::utils::newGrid<int>(30, 30, 0);

    auto automaton = ca::par::CellularAutomaton<int>(grid, 30, 30, conways_game_of_life_update_function<int>);

    automaton.simulate(10);

    ca::utils::deleteGrid(grid, 30);
}