# tsp
An implementation of the Held-Karp dynamic programming algorithm to solve the Traveling Salesman Problem

- Parallelization via OpenMP (see Makefile)
- Linux (e. g. `pacman -S openmp`) and macOS (`brew install llvm`)
- `ACTUAL_N` is the number of vertices to incorporate. Adapt your `dist_matrix` accordingly.

On my machines, 28 vertices are processed in 1:20 to 1:30 mins with < 8 GB memory usage.
