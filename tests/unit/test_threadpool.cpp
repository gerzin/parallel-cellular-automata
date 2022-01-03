#include <iostream>
#include <threadpool.hpp>
#include <utimer.hpp>
#include <vector>
using namespace ca;
int main()
{
    std::atomic<unsigned> counter = 0;
    {
        UTimer timer("Section");
        ca::Threadpool pool(4);
        std::cout << pool.get_number_workers() << std::endl;

        auto fun = [&](int i) { counter += i; };

        std::vector<std::future<void>> results;
        results.reserve(10000);

        for (int i = 1; i <= 10000; i++)
        {
            results.push_back(pool.submit(fun, i));
        }

        for (auto &r : results)
        {
            r.wait();
        }
    }
    std::cout << counter << std::endl;

    return EXIT_SUCCESS;
}