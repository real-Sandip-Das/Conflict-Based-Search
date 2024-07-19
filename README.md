# Conflict-Based-Search

Attempted implementation of the Conflict Based Search algorithm for MAPF(Multi Agent Path Finding)

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
```

Note:

- Depending on the number of agents to be solved for, the program may take a lot of time to run (around 10-15 agents at a time runs fine on my setup)
