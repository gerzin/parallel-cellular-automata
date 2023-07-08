/**
 * @file cellular_automata.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This header imports all the other headers of the framework.
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#define PARALLEL_CELLULAR_AUTOMATA_CELLULAR_AUTOMATA_HPP
#include <iostream>
// clang-format off
#include "grid.hpp"
#include "sequential_automaton.hpp"
#include "parallel_automaton.hpp"
#include "parallel_automaton_busyw.hpp"
#include "omp_automaton.hpp"
#include "ff_automaton.hpp"

#ifdef __NVCC__
#include "cuda_automaton.hpp"
#endif

// clang-format on

/**
 * @brief Namespace of the framework.
 *
 */
namespace ca
{
/**
 * @brief Interface of the CellularAutomaton
 */
class AbstractCellularAutomaton
{
  public:
    /**
     * @brief Run the simulation for a given number of steps.
     *
     * @param steps number of simulation steps to run.
     *
     */
    virtual void simulate(unsigned steps = 1) = 0;
    /**
     * @brief Get the generation of the simulation.
     *
     * @return size_t value of the generation member variable.
     */
    virtual size_t get_generation() = 0;
};

constexpr bool is_cuda_available()
{
#ifdef __CUDACC__
    return true;
#else
    return false;
#endif
}

} // namespace ca

#endif