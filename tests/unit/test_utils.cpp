#include "../../include/cellular_automata.hpp"
#include "../../include/grid.hpp"

#include <iostream>
using namespace ca;
using namespace std;

int main()
{
    ca::Grid<int> grid(10, 10);
    cout << grid << endl;

    auto b = ca::Grid<int>::newWithSameSize(grid);

    for (auto i = 0; i < 10; ++i)
    {
        b[i][0] = 1;
    }

    std::swap(grid, b);

    cout << grid << endl;

    cout << "*********************" << endl;

    cout << b << endl;
}