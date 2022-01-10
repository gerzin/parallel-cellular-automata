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

namespace ca
{
/**
 * @brief Work-stealing threadpool.
 *
 */
class Threadpool
{

    using QueuesContentType = std::function<void()>;

  public:
    /**
     * @brief Construct a new Threadpool object.
     *
     * @param nw number of workers. If 0 hardwareconcurrency is used. (default 0)
     */
    Threadpool(unsigned nw = 0);

    /**
     * @brief Destroy the Threadpool object.
     *
     */
    ~Threadpool();

    /**
     * @brief Get the number of worker threads.
     *
     * @return size_t the number of worker threads.
     */
    size_t get_number_workers() const;

    /**
     * @brief Submit work to the threadpool. This function takes a function and its arguments and returns a future that
     * will contain the return value of the function. This can be used to wait for the threadpool to run the task.
     *
     * @tparam F function type.
     * @tparam Args args type.
     * @param f function to execute.
     * @param args argument to pass to the function.
     * @return std::future<typename std::result_of<F(Args...)>::type> future that will contain the result of the
     * function.
     */
    template <class F, class... Args>
    auto submit(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        // type returned by f(args)
        using return_value_type = typename std::result_of<F(Args...)>::type;

        /*
         * std::bind creates a function wrapper "g" s.t. g == f(args)
         */
        auto task = std::make_shared<std::packaged_task<return_value_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        // with the packaged task we split the execution from the return value
        std::future<return_value_type> future_result(task->get_future());

        if (local_queue)
        {
            // to avoid cache ping-pong and improve performances each thread pushes on its individual queue if it can.
            local_queue->push([task]() {
                // dereference the package
                (*task)();
            });
        }
        else
        {
            threadpool_work_queue.push([task]() { (*task)(); });
        }

        return future_result;
    }

  private:
    std::atomic<bool> done;
    ThreadSafeQueue<QueuesContentType> threadpool_work_queue;
    std::vector<std::unique_ptr<WorkStealingQueue<QueuesContentType>>> workers_queues;
    std::vector<std::thread> threads;
    ThreadJoiner joiner;

    // Try to steal the work from other thread's queues.
    bool try_stealing_work(QueuesContentType &result)
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

    // thread-local variables containing the thread index and the local queue.
    static thread_local WorkStealingQueue<QueuesContentType> *local_queue;
    static thread_local unsigned thread_index;

    // work stealing worker
    void worker_thread(unsigned index);
};
} // namespace ca

#endif