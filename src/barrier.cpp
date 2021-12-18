#include <barrier.hpp>
#include <iostream>
#include <stdexcept>

ca::Barrier::Barrier(unsigned n) : m(), cond(), n_threads(n), count(n), direction(Direction::DECREASING)
{
    if (!n)
    {
        throw std::invalid_argument("The number of threads cannot be zero.");
    }
}

void ca::Barrier::wait()
{
    std::unique_lock<std::mutex> lock{m};
    if (direction == DECREASING)
    {
        // counting down
        if (--count) // > 0
        {
            // wait for a change of direction. (All other threads reached the barrier)
            cond.wait(lock, [this] { return direction == Direction::INCREASING; });
        }
        else
        {
            // I'm the last thread to reach the barrier.
            direction = INCREASING;
            cond.notify_all();
        }
    }
    else
    {
        // counting up
        if (++count < n_threads)
        {
            // wait for a change of direction. (All other threads reached the barrier)
            cond.wait(lock, [this] { return direction == Direction::DECREASING; });
        }
        else
        {
            // I'm the last thread to reach the barrier.
            direction = DECREASING;
            cond.notify_all();
        }
    }
}