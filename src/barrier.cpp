/**
 * @file barrier.cpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief Implementation of the barrier defined in barrier.hpp
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
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
        if (--count)
        {
            // wait for a change of direction. (All other threads reached the barrier)
            cond.wait(lock, [this] { return this->direction == INCREASING; });
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
        if (++count < n_threads)
        {
            // wait for a change of direction. (All other threads reached the barrier)
            cond.wait(lock, [this] { return this->direction == DECREASING; });
        }
        else
        {
            direction = DECREASING;
            cond.notify_all();
        }
    }
}

void ca::Barrier::wait(std::function<void()> fun)
{
    std::unique_lock<std::mutex> lock{m};

    if (direction == DECREASING)
    {
        if (--count)
        {
            cond.wait(lock, [this] { return this->direction == INCREASING; });
        }
        else
        {
            fun();
            direction = INCREASING;
            cond.notify_all();
        }
    }

    else
    {

        if (++count < n_threads)
        {

            cond.wait(lock, [this] { return this->direction == DECREASING; });
        }
        else
        {

            fun();
            direction = DECREASING;
            cond.notify_all();
        }
    }
}

void ca::Barrier::busy_wait()
{
    std::unique_lock<std::mutex> lock{m};

    if (direction == DECREASING)
    {
        if (--count)
        {
            lock.unlock();
            // busy wait for a change of direction. (All other threads reached the barrier)
            while (direction == DECREASING)
            {
                ;
            }
        }
        else
        {
            direction = INCREASING;
            cond.notify_all();
        }
    }

    else
    {

        if (++count < n_threads)
        {
            lock.unlock();
            // busy wait for a change of direction. (All other threads reached the barrier)
            while (direction == INCREASING)
            {
                ;
            }
        }
        else
        {

            direction = DECREASING;
            cond.notify_all();
        }
    }
}

void ca::Barrier::busy_wait(std::function<void()> fun)
{
    std::unique_lock<std::mutex> lock{m};

    if (direction == DECREASING)
    {
        if (--count)
        {
            lock.unlock();
            // busy wait for a change of direction. (All other threads reached the barrier)
            while (direction == DECREASING)
            {
                ;
            }
        }
        else
        {
            fun();
            direction = INCREASING;
            cond.notify_all();
        }
    }

    else
    {
        if (++count < n_threads)
        {
            lock.unlock();
            while (direction == INCREASING)
            {
                ;
            }
        }
        else
        {
            fun();
            direction = DECREASING;
            cond.notify_all();
        }
    }
}