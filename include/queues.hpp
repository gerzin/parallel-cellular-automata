/**
 * @file queues.hpp
 * @author Gerardo Zinno (gerardozinno1@gmail.com)
 * @brief This file contains the definition and implementation of some useful thread-safe queues.
 * @version 0.2
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef PARALLEL_CELLULAR_AUTOMATA_WORKSTEALINGQUEUE_HPP
#define PARALLEL_CELLULAR_AUTOMATA_WORKSTEALINGQUEUE_HPP
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>

/**
 * @brief A thread-safe wrapper around a queue.
 *
 * @tparam T
 */
template <typename T>
class ThreadSafeQueue
{

  public:
    /**
     * @brief Construct a new ThreadSafeQueue object.
     *
     */
    ThreadSafeQueue();
    /**
     * @brief Construct a new ThreadSafeQueue object.
     *
     * @param other queue to copy.
     */
    ThreadSafeQueue(ThreadSafeQueue const &other)
    {
        std::lock_guard<std::mutex> lock(m);
        queue = other.queue;
    }
    /**
     * @brief Insert an element into the queue.
     *
     * @param elem element to insert.
     */
    void push(T elem)
    {
        std::lock_guard<std::mutex> lock(m);
        queue.push(elem);
        cond.notify_one();
    }
    /**
     * @brief Try to pop an element from the queue.
     *
     * @param result reference where to put the popped element.
     * @return true if the element has been popped.
     * @return false if the queue was empty.
     */
    bool try_pop(T &result)
    {
        std::lock_guard<std::mutex> lock(m);
        if (queue.empty())
        {
            return false;
        }
        else
        {
            result = queue.front();
            queue.pop();
            return true;
        }
    }
    /**
     * @brief Try to pop an element from the queue.
     *
     * @return std::shared_ptr<T> pointer containing the popped element or an empty pointer if the queue was empty.
     */
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (queue.empty())
        {
            return std::shared_ptr<T>();
        }
        else
        {
            auto result = std::make_shared<T>(queue.front());
            queue.pop();
            return result;
        }
    }
    /**
     * @brief Pop an element from the queue.
     *
     * @param result reference where to put the popped element.
     */
    void pop(T &result)
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, [this] { return !queue.empty(); });
        result = queue.front();
        queue.pop();
    };
    /**
     * @brief Pop an element from the queue.
     *
     * @return std::shared_ptr<T> pointer containing the popped element.
     */
    std::shared_ptr<T> pop()
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, [this] { return !queue.empty(); });
        auto result = std::make_shared<T>(queue.front());
        queue.pop();
        return result;
    }
    /**
     * @brief Check if the queue is empty.
     * @note  The result may be outdated.
     *
     * @return true if the queue is empty.
     * @return false if the queue is not empty.
     */
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return queue.empty();
    }

  private:
    // mutable so it can be locked in empty and in the copy constructor
    mutable std::mutex m;
    std::queue<T> queue;
    std::condition_variable cond;

}

/**
 * @brief work-stealing queue.
 *
 * This work-stealing queue is implemented as a simple wrapper around a deque.
 *
 * To improve performance from a cache locality perspective the queue act as a LIFO
 * for its own thread and as a FIFO for the stealing one.
 *
 * @tparam T
 */
template <typename T>
class WorkStealingQueue
{

  public:
    /**
     * @brief Construct a new WorkStealingQueue object.
     *
     */
    WorkStealingQueue();
    WorkStealingQueue(const WorkStealingQueue &other) = delete;
    /**
     * @brief Push data into the queue.
     *
     * @param data data to push.
     */
    void push(T data)
    {
        std::lock_guard<std::mutex> lock(m);
        queue.push_front(std::move(data));
    }
    /**
     * @brief Check if the queue is empty.
     *
     * @return true if the queue is empty.
     * @return false if the queue is not empty.
     */
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return queue.empty();
    }
    /**
     * @brief Try to pop an element from the front of the queue.
     *
     * @param result reference where to insert the popped element.
     * @return true if the element has been popped.
     * @return false if the queue was empty.
     */
    bool try_pop(T &result)
    {
        std::lock_guard<std::mutex> lock(m);
        if (queue.empty())
        {
            return false
        }
        result = std::move(queue.front());
        queue.pop_front();
        return true;
    }
    /**
     * @brief Try to pop an element from the front of the queue.
     *
     * @param result reference where to insert the popped element.
     * @return true if the element has been popped.
     * @return false if the queue was empty.
     */
    bool try_steal(T &result)
    {
        std::lock_guard<std::mutex> lock(m);
        if (queue.empty())
        {
            return false
        }
        result = std::move(queue.back());
        queue.pop_back();
        return true;
    }

  private:
    mutable std::mutex m;
    std::deque<T> queue;
};
#endif