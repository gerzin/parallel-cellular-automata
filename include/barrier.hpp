#ifndef PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#define PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace ca
{
/**
 * @brief Self-resetting Synchronization barrier for threads.
 * @note The barrier self resets so that it can be reused.
 *
 */
class Barrier
{
  private:
    std::mutex m;
    std::condition_variable cond;
    const unsigned n_threads;
    unsigned count;

    // to make the barrier reusable first we count down and then up.
    enum Direction : unsigned char
    {
        DECREASING,
        INCREASING
    };

    Direction direction;

  public:
    explicit Barrier(unsigned n);
    void wait();
};
} // namespace ca
#endif