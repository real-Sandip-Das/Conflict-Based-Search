# Conflict-Based-Search

An implementation of the Conflict Based Search algorithm for MAPF(Multi Agent Path Finding) with a web visualizer.

![Example visualization](output.gif)

>Note: The visualizer can be run without compiling because it already contains a `solution.json`

## Local Setup

1. Install `vcpkg` (e.g., at `~/vcpkg`):

   ```sh
   git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
   ~/vcpkg/bootstrap-vcpkg.sh
   ```

2. Building the C++ source code:

   ```sh
   sh build.sh
   ```

   > **Note:** `build.sh` assumes by default that `vcpkg` is installed at `~/vcpkg`. If you installed `vcpkg` in a different directory, you can either set the `VCPKG_ROOT` environment variable (`export VCPKG_ROOT=/path/to/vcpkg`) before running the script, or manually edit `build.sh` to update the path.

3. Run the tests:

   ```sh
   ./cpp_files/cmake-build-debug/cbs_tests
   ```

## Using Docker Compose

1. Build and start the visualizer in the background:

   ```sh
   docker compose up -d visualizer
   ```

2. Running the planner:

   ```sh
   docker compose run --rm solver
   ```

3. Running the tests (optional):

   ```sh
   docker build -t cbs-project .
   docker run --rm cbs-project /bin/sh -c "cd cpp_files/cmake-build-debug && ./cbs_tests"
   ```

## Generating Documentation

Doxygen configuration is given in the `Doxyfile` to generate documentation from the C++ source files.

To generate the documentation locally:

1. Ensure `doxygen` and `graphviz` are installed (e.g., `sudo apt install doxygen graphviz` on Debian/Ubuntu).
2. Run Doxygen from the repository root:
   ```sh
   doxygen Doxyfile
   ```
3. Open `doc/html/index.html` in your web browser to view the generated docs.

## Running the Visualizer locally

After getting the `solution.json` file using the `ConflictBasedSearch` executable, you can visualize the paths in your browser.

1. Start a local server:

   ```sh
   cd visualizer
   python3 -m http.server 8080
   ```

2. Open your web browser and go to `http://localhost:8080`.

## Benchmarks

The path planner was benchmarked using the test in `test_benchmark.cpp`.

| Number of Agents | Execution Time | Status |
| :---: | :---: | :---: |
| 1 | 10 ms | Optimal Path Found |
| 2 | 9 ms | Optimal Path Found |
| 3 | 11 ms | Optimal Path Found |
| 5 | 18 ms | Optimal Path Found |
| 8 | 29 ms | Optimal Path Found |
| 10 | 36 ms | Optimal Path Found |
| 12 | 397 ms | Optimal Path Found |
| 15 | 498 ms | Optimal Path Found |
| 20 | 1115 ms | Optimal Path Found |
| 25 | Timeout (> 4 min) | Conflict space too dense |
