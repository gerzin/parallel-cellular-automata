## A simple C++ framework for building fast cellular automata on a toroidal surface.

### Instructions
To use the framework you simply need to:
1. Create a grid and initialize it:

```
ca::Grid<int> g(1000,1000);
for(int i = 0; i < 1000; ++i){
  for(int j = 0; j < 1000; j++){
    g(i,j) = something;
  }
}
```
2. Define an update function taking the central cell as first argument and its neighbors as subsequent arguments:

```
// tl = top_left cell, t = top cell, ...
auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) {
                     do_something;
                 };
```

3. Create one of the four available cellular automaton:

```
ca::seq::CellularAutomaton<int>(g, update_fn);
// to these four you can pass also the number of worker threads as last argument (default = hardware_concurrency)
ca::par::CellularAutomaton<int> aut(g, update_fn);
ca::par::bw::CellularAutomaton<int> aut(g, update_fn);
ca::ffl::CellularAutomaton<int> aut(g, update_fn);
ca::omp::CellularAutomaton<int> aut(g, update_fn);
```

4. Run n simulation steps:

```
aut.simulate(n);
```

5. Print the grid:

```
std::cout << g << std::endl;
```


