#include <barrier.hpp>
#include <stdexcept>

ca::Barrier::Barrier(unsigned n) : nthreads{n}, spaces{n}, generation{0}
{
    if (!n)
    {
        throw std::invalid_argument("The number of threads cannot be zero.");
    }
}

void ca::Barrier::wait()
{
    unsigned const current_gen{generation};
    if (!--spaces)
    {
        // increase generation and reset
        ++generation;
        spaces = nthreads;
    }
    else
    {
        while (generation == current_gen)
        {
            std::this_thread::yeld(); // so the waiting thread doesn't waste CPU time while waiting
        }
    }
}