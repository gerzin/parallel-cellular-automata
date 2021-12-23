/**
 * @file threadpool.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief definition of a threadpool.
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_THREADPOOL_HPP
#define PARALLEL_CELLULAR_AUTOMATA_THREADPOOL_HPP
#include <atomic>
#include <memory>
#include <queues.hpp>
#include <thread>
#include <vector>

/**
 * @brief RAII class to join threads.
 *
 */
class ThreadJoiner
{
  private:
    std::vector<std::thread> &threads;

  public:
    ThreadJoiner(std::vector<std::thread> &t);
    ~ThreadJoiner();
}

namespace ca
{
    /**
     * @brief Work-stealing threadpool.
     *
     * @tparam T
     */
    template <typename T>
    class Threadpool
    {
      public:
        /**
         * @brief Construct a new Threadpool object.
         *
         * @param nw number of workers. If 0 hardwareconcurrency is used. (default 0)
         */
        Threadpool(unsigned nw = 0) : done(false), joiner(threads)
        {
            unsigned nthreads = nw;
            if (nthreads)
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
                    workers_queues.emplace_back(new WorkStealingQueue<T>);
                    threads.emplace_back(&Threadpool::worker_thread, this, i);
                }
            }
            catch (...)
            {
                done = true;
                throw;
            }
        }
        ~Threadpool()
        {
            // the join is done by the ThreadJoiner.
            done = true;
        }

        void submit()
        {
            // TODO implement
        }

      private:
        std::atomic<bool> done;
        ThreadSafeQueue<T> threadpool_work_queue;
        std::vector < std::unique_ptr<WorkStealingQueue<T>> workers_queues;
        std::vector<std::thread> threads;
        ThreadJoiner joiner;

        // thread-local
        static thread_local WorkStealingQueue *local_queue;
        static thread_local unsigned thread_index;

        // Try to steal the work from other thread's queues.
        bool try_stealing_work(T &result)
        {
            unsigned qsize = workers_queues.size();
            for (unsigned i = 0; i < qsize; ++i)
            {
                unsigned const index = (thread_index + 1) % qsize;
                if (workers_queues[index]->try_steal(T))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        // work stealing worker
        void worker_thread(unsigned index)
        {
            thread_index = index;
            local_queue = workers_queues[index].get();
            while (!done)
            {
                T task;
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
    };
} // namespace ca

#endif