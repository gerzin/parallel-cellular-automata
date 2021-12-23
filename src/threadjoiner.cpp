/**
 * @file threadjoiner.cpp
 * @author Gerardo Zinno (g.zinno@studenti.unipi.it)
 * @brief This file contains the implementation of the ThreadJoiner defined in threadpool.hpp
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <threadpool.hpp>

ThreadJoiner::ThreadJoiner(std::vector<std::thread> &t) : threads(t){};
ThreadJoiner::~ThreadJoiner()
{
    for (size_t i{0}; i < threads.size(); ++i)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }
}