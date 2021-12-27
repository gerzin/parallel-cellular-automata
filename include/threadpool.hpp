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
#include <future>
#include <memory>
#include <queues.hpp>
#include <thread>
#include <type_traits>
#include <vector>

/**
 * @brief RAII class to join threads.
 * @see threadjoiner.cpp for the implementation
 */
class ThreadJoiner
{
  private:
    std::vector<std::thread> &threads;

  public:
    /**
     * @brief Construct a new ThreadJoiner object.
     *
     * @param t vector containing the threads to join.
     */
    ThreadJoiner(std::vector<std::thread> &t);
    /**
     * @brief Destroy the Thread Joiner object and in doing this join all the threads in the vector.
     *
     */
    ~ThreadJoiner();
};

namespace // unnamed namespace to keep this names inside this unit.
{

/**
 * @brief Callable class that wraps a function. Will contain the tasks that will be run by the threadpool.
 *
 */
class Callable
{
  private:
    // abstract callable class for the smart pointer
    struct abstract_callable
    {
        virtual void call() = 0;
        virtual ~abstract_callable(){};
    };

    std::unique_ptr<abstract_callable> callable;

    // concrete function wrapper
    template <typename Function>
    struct concrete_callable : abstract_callable
    {
      public:
        Function f;
        concrete_callable(Function &&fun) : f(std::move(fun))
        {
        }

        void call()
        {
            f();
        }
    };

  public:
    template <typename Function>
    Callable(Function &&fun) : callable(new concrete_callable<Function>(std::move(fun)))
    {
    }

    void operator()()
    {
        callable->call();
    }

    Callable() = default;

    Callable(Callable &&other) : callable(std::move(other.callable))
    {
    }

    Callable &operator=(Callable &&other)
    {
        callable = std::move(other.callable);
        return *this;
    }

    Callable &operator=(const Callable &) = delete;

    Callable(const Callable &other) = delete;

    Callable(Callable &other) = delete;
};

} // namespace
namespace ca
{
/**
 * @brief Work-stealing threadpool.
 *
 */
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
                workers_queues.emplace_back(new WorkStealingQueue<Callable>);
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

    /**
     * @brief Get the number of worker threads.
     *
     * @return size_t the number of worker threads.
     */
    size_t get_number_workers() const
    {
        return threads.size();
    }

    template <typename Function>
    auto submit(Function f)
    {
        // type of the result returned by f
        std::packaged_task<typename std::result_of<Function()>::type> task(std::move(f));
        auto result = task.get_future();
        threadpool_work_queue.push(std::move(task));
        return result;
    }

  private:
    std::atomic<bool> done;
    ThreadSafeQueue<Callable> threadpool_work_queue;
    std::vector<std::unique_ptr<WorkStealingQueue<Callable>>> workers_queues;
    std::vector<std::thread> threads;
    ThreadJoiner joiner;

    // Try to steal the work from other thread's queues.

    bool try_stealing_work(Callable &result)
    {
        unsigned qsize = workers_queues.size();
        for (unsigned i = 0; i < qsize; ++i)
        {
            unsigned const index = (thread_index + 1) % qsize;
            if (workers_queues[index]->try_steal(result))
            {
                return true;
            }
        }
        return false;
    }

    // thread-local
    static thread_local WorkStealingQueue<Callable> *local_queue;
    static thread_local unsigned thread_index;

    // work stealing worker
    void worker_thread(unsigned index)
    {
        thread_index = index;
        local_queue = workers_queues[index].get();
        while (!done)
        {
            Callable task;
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