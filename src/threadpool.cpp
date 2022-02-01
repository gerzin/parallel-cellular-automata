/**
 * @file threadpool.cpp
 * @author Gerardo Zinno (g.zinno@studenti.unipi.it)
 * @brief This file contains the implementation of the classes defined in threadpool.hpp
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>
#include <threadpool.hpp>
/*
 * THREAD JOINER
 */

ThreadJoiner::ThreadJoiner(std::vector<std::thread> &t) : threads(t)
{
}

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

/*
 * THREADPOOL
 */

thread_local unsigned ca::Threadpool::thread_index = 0;

thread_local WorkStealingQueue<std::function<void()>> *ca::Threadpool::local_queue = nullptr;

ca::Threadpool::Threadpool(unsigned nw) : done(false), joiner(threads)
{

    unsigned nthreads = nw;
    if (!nthreads)
    {
        nthreads = std::thread::hardware_concurrency();
    }
    workers_queues.reserve(nthreads);
    threads.reserve(nthreads);
    // starting threads.
    try
    {
        for (unsigned i{0}; i < nthreads; ++i)
        {
            workers_queues.emplace_back(new WorkStealingQueue<QueuesContentType>);
            threads.emplace_back(&Threadpool::worker_thread, this, i);
        }
    }
    catch (...)
    {
        done = true;
        throw;
    }
}

size_t ca::Threadpool::get_number_workers() const
{
    return threads.size();
}

ca::Threadpool::~Threadpool()
{
    // the join is done by the ThreadJoiner.
    done = true;
}

void ca::Threadpool::worker_thread(unsigned index)
{
    thread_index = index;
    local_queue = workers_queues[index].get();

    while (!done)
    {
        QueuesContentType task;
        if ((local_queue && local_queue->try_pop(task)) // try pop from local
            || threadpool_work_queue.try_pop(task)      // try pop from threadpool pool
            || try_stealing_work(task)                  // try to steal work from other threads
        )
        {
            // now task contains something executable.
            task();
        }
        else
        {
            // give-up the CPU to another thread so progress can be made.
            std::this_thread::yield();
        }
    }
}
