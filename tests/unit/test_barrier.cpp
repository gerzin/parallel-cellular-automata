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
    static unsigned counter = 0;
    unsigned nthreads = 20;
    ca::Barrier sync_point(nthreads);

    SECTION("There are no deadlocks"){

        auto thread_fn = [&]() {  
            { 
                std::lock_guard<std::mutex> lg {m};
                ++counter; 
            } 
            sync_point.wait();
        };

        std::vector<std::thread> threads;
        threads.reserve(nthreads);
        for(unsigned i = 0; i < nthreads; ++i){
            threads.emplace_back(thread_fn);
        }
        for(auto& t: threads) {
            t.join(); 
        }
        REQUIRE(counter == nthreads);
    }

    SECTION("The same barrier can be used multiple times") {
        auto thread_fn = [&]() {  
            {
                std::lock_guard<std::mutex> lg {m};
                ++counter;
            }
            sync_point.wait();
        };

        std::vector<std::thread> threads;
        threads.reserve(nthreads);
        
        for(unsigned i = 0; i < nthreads; ++i){
            threads.emplace_back(thread_fn);
        }

        for(auto& t: threads) {
            t.join();
        }

        REQUIRE(counter == 2*nthreads);

    }

    SECTION("The last thread will call the function") {
        auto thread_fn = [&]() {  
            {
                std::lock_guard<std::mutex> lg {m};
                ++counter;
            }
            sync_point.wait([](){
                std::cout << "I'm the last thread to reach the barrier! (You should see this message exactly one time)" << std::endl;
            });
        };

        std::vector<std::thread> threads;
        threads.reserve(nthreads);
        
        for(unsigned i = 0; i < nthreads; ++i){
            threads.emplace_back(thread_fn);
        }

        for(auto& t: threads) {
            t.join();
        }

        REQUIRE(counter == 3*nthreads);
    }
    
    
}


 