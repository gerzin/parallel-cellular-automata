/**
 * @file barrier.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This file contains the definition of a barrier for thread synchronization.
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#define PARALLEL_CELLULAR_AUTOMATA_BARRIER_HPP
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace ca
{
/**
 * @brief Self-resetting synchronization barrier for threads.
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
    /**
     * @brief Construct a new Barrier object to syncronize n threads.
     *
     * @param n number of threads to syncronize.
     * @throws invalid_argument if n is equal to zero.
     */
    explicit Barrier(unsigned n);
    /**
     * @brief wait for the other threads to reach the barrier.
     *
     */
    void wait();
    /**
     * @brief wait for the other treads to reach the barrier and exec fun bebore notyfying others if you are the last
     * thread to reach it.
     *
     * @param fun function that the last thread to reach the barrier will execute before notifying.
     */
    void wait(std::function<void()> fun);

    /**
     * @brief busy wait for other threads to reach the barrier.
     *
     */
    void busy_wait();

    /**
     * @brief busy wait for the other treads to reach the barrier and exec fun bebore notyfying others if you are the
     * last thread to reach it.
     *
     * @param fun function that the last thread to reach the barrier will execute before notifying.
     */
    void busy_wait(std::function<void()> fun);
};
} // namespace ca
#endif