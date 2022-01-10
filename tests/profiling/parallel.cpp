#include <cellular_automata.hpp>
#include <functional>

using namespace std;
int main()
{
    ca::Grid<int> g(100, 100);
    // setting some grid cells
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; j += 2)
        {
            g(i, j) = i + j;
        }
    }
    // define a function
    auto fun = [](int c, int tl, int tr, int t, int l, int r, int bl, int b, int br) {
        return tl + tr + t + l + r + bl + b + br - c;
    };

    auto automaton = ca::par::CellularAutomaton<int>(g, fun, 4);

    automaton.simulate(10);
}