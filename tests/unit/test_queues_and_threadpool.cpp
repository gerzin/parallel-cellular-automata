// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include <climits>
#include <queues.hpp>
#include <threadpool.hpp>

using namespace std;

void simulate_work(unsigned millis)
{
    this_thread::sleep_for(std::chrono::milliseconds(millis));
}

TEST_CASE("The thread-safe queues work")
{
    SECTION("Thread-Safe queue")
    {
        ThreadSafeQueue<int> queue;
        std::atomic<int> count{0};
        {

            auto pusher = [&]() {
                for (int i{0}; i < 5; ++i)
                {
                    queue.push(i);
                    count++;
                    simulate_work(3);
                }
            };

            auto popper = [&]() {
                int result;
                while (count > 0)
                {

                    if (queue.try_pop(result))
                    {
                        count--;
                    }
                    else
                    {
                        this_thread::yield();
                    }
                }
            };

            vector<thread> threads;
            ThreadJoiner joiner(threads);

            for (unsigned i{0}; i < 100; ++i)
            {
                threads.push_back(thread(pusher));
                if (!(i % 10))
                {
                    threads.emplace_back(popper);
                }
            }
        }
    }

    SECTION("WorkStealingQueue")
    {
        std::atomic<int> count{0};
        WorkStealingQueue<int> queue;

        {
            vector<thread> threads;
            ThreadJoiner joiner(threads);

            auto pusher = [&]() {
                for (int i{0}; i < 5; ++i)
                {
                    queue.push(i);
                    simulate_work(3);
                }
            };

            auto popper = [&](int nelem) {
                int result;
                while (nelem)
                {
                    if (queue.try_pop(result))
                    {
                        nelem--;
                    }
                    else
                    {
                        simulate_work(4);
                    }
                }
            };

            auto stealer = [&](int nelem) {
                int result;
                while (nelem)
                {
                    if (queue.try_steal(result))
                    {
                        nelem--;
                    }
                    else
                    {
                        this_thread::yield();
                    }
                }
            };

            // 500 elements will be inserted.
            for (unsigned i{0}; i < 100; ++i)
            {
                threads.push_back(thread(pusher));
                if (i % 10 == 0)
                {
                    if (i % 5)
                    {
                        threads.emplace_back(stealer, 50);
                    }
                    else
                    {
                        threads.emplace_back(popper, 50);
                    }
                }
            }
        }
        REQUIRE(count == 0);
    }
}

TEST_CASE("The threadpool works correctly")
{
    std::atomic<unsigned> counter;
    ca::Threadpool pool;

    REQUIRE(pool.get_number_workers() == std::thread::hardware_concurrency());

    auto fun = [&](int i) { counter += i; };

    for (int i = 0; i < 10000; i++)
    {
        pool.submit(fun, i);
    }
}