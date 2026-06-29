# Conflict-Based-Search

Attempted implementation of the Conflict Based Search algorithm for MAPF(Multi Agent Path Finding)

![Example visualization](output.gif)

## Building

Building the project requires OpenCV Library and Headers preinstalled along with `g++` and `cmake`

In a Linux based System, `build.sh` can be run to build the project

```sh
sh build.sh
```

## Running

It's necessary to run the executable from the right directory relative to the `assets` directory

`run.sh` takes care of that:

```sh
sh run.sh
cd visualizer
python3 -m http.server 8080
```

The visualizer directory already contains an output from the solver to be visualized.

## Benchmarks

The solver was benchmarked on the `ost003d` map using a random scenario:

| Number of Agents | Solver Execution Time | Status |
| :---: | :---: | :---: |
| 1 | 3 ms | Optimal Path Found |
| 2 | 5 ms | Optimal Path Found |
| 3 | 8 ms | Optimal Path Found |
| 5 | 13 ms | Optimal Path Found |
| 8 | 20 ms | Optimal Path Found |
| 10 | 24 ms | Optimal Path Found |
| 12 | 268 ms | Optimal Path Found |
| 15 | 343 ms | Optimal Path Found |
| 20 | 733 ms | Optimal Path Found |
| 25 | Timeout (> 4 min) | Conflict space too dense |
