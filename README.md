# parallel-cellular-automata

Framework for building Cellular Automata developed as a project for the exam of Parallel and Distrituted Systems.
It offers four types of automata:
* A sequential one
* Three parallel ones, parallelized using:
    * standard c++ library and pthread
    * OpenMp
    * [FastFlow](https://github.com/fastflow/fastflow) framework

the framework is inside the namespace `ca` and each automaton is located in its respective nested namespace (`ca::seq` , `ca::par`, `ca::ffl`, `ca::omp`).

The interfaces of the automata are the same, so you can try different ones by simply replacing the namespace.
The sequential automaton is missing the parameter to set the number of threads. If the other automata are called without passing that parameter, it will be set by default to `std::thread::hardware_concurrency`.

#### Note on fastflow
To optimize it run the script `mapping_string.sh` under `include/ff` (you need to have hwloc installed).
This will be done automatically by cmake when you build the library.
# Compilation
to compile the library you must use CMake.

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

this will create a static library in the build directory. To use the framework include the files under `include` and link with the generated library (`libparallelcellularautomata.a` on linux).

All the documentation is written using doxygen comments. Call `doxygen` in the main dir to generate it (this will create a `doc` folder).

## Example
```c++
#include<cellular_automata.hpp>

int main() {
    ca::Grid<int> grid(100,100);

    // init the grid
    for(...){
        grid(i,j) = something;
    }

    // define update function with this signature
    auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) {
        do_something;
    };

    // if the namespace is the one of a parallel implementation you can pass
    // the number of workers too.
    ca::(seq,par,omp,ffl)::CellulatAutomaton<int> aut(grid, update_fn);
    
    // run 10 simulation steps
    aut.simulate(10);

    std::cout << grid;

}
```