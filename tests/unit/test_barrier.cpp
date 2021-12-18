// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <barrier.hpp>
#include <vector>
#include <iostream>

TEST_CASE("The Barrier works correctly") {
    std::mutex m;
    int counter = 0;
    unsigned nthreads = 20;
    ca::Barrier sync_point(nthreads);
    auto thread_fn = [&](int i) {
        std::lock_guard<std::mutex> lg {m};
        ++counter;
        std::cout << "Thread " << i << " waits at the barrier" << std::endl;
        sync_point.wait();
        std::cout << "Thread " << i << "restarts" << std::endl;
    };
    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    for(unsigned i = 0; i < nthreads; ++i){
        threads.emplace_back(thread_fn, i);
    }
    for(auto& t: threads) {
        t.join();
    }

}


