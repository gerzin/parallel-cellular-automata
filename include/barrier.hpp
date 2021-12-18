#ifndef PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#define PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#include <thread>
#include <vector>

namespace ca
{
class Barrier
{
  private:
    unsigned const nthreads;
    std::atomic<unsigned> spaces;
    std::atomic<unsigned> generation;

  public:
    explicit Barrier(unsigned n);
    void wait();
};
} // namespace ca
#endif