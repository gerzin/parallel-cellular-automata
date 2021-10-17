#ifndef PARALLEL_CELLULAR_AUTOMATA_CA_GRID_HPP
#define PARALLEL_CELLULAR_AUTOMATA_CA_GRID_HPP
#include "sequential_automaton.hpp"
#include <iostream>

namespace ca
{

namespace seq
{
template <typename T>
class CAGrid
{
    friend class CellularAutomaton<T>;

  public:
  
  protected:
    T **grid;
    size_t rows;
    size_t columns;
};
} // namespace seq
} // namespace ca

#endif