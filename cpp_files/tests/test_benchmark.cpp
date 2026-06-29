#include <gtest/gtest.h>
#include <fstream>
#include <chrono>
#include <sstream>
#include <vector>
#include "../include/ConvertMap.h"
#include "../include/AStar.h"
#include "../include/ConflictTree.h"

// Helper to locate the assets directory dynamically
std::string find_assets_dir() {
    std::vector<std::string> candidates = {
        "../../assets/",
        "../assets/",
        "./assets/",
        "../../../assets/"
    };
    for (const auto& path : candidates) {
        std::ifstream f(path + "ost003d.map");
        if (f.good()) {
            return path;
        }
    }
    return "";
}

// Helper to read the scenario file without OpenCV dependencies
void load_scenario_coords(const std::string& scenario_path, int n_agents, std::vector<point>& starts, std::vector<point>& goals) {
    std::ifstream file(scenario_path);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    std::getline(file, line); // version 1
    starts.clear();
    goals.clear();
    for (int i = 0; i < n_agents; ++i) {
        if (!std::getline(file, line)) break;
        std::istringstream iss(line);
        int bucket_dummy;
        std::string map_name_dummy;
        int width_dummy, height_dummy, xs, ys, xg, yg;
        float path_length_dummy;
        if (iss >> bucket_dummy >> map_name_dummy >> width_dummy >> height_dummy >> xs >> ys >> xg >> yg >> path_length_dummy) {
            starts.push_back({xs, ys});
            goals.push_back({xg, yg});
        }
    }
}

TEST(CBSBenchmark, Performance) {
    std::string assets_dir = find_assets_dir();
    ASSERT_NE(assets_dir, "") << "Could not find assets directory containing ost003d.map";

    std::string map_path = assets_dir + "ost003d.map";
    std::string scen_path = assets_dir + "ost003d.map-scen-random/scen-random/ost003d-random-1.scen";

    auto map_result = convert_map(map_path);
    ASSERT_TRUE(map_result.has_value()) << "Failed to parse map: " << map_path;
    auto map_arr = map_result.value();

    std::vector<int> agent_counts = {1, 2, 3, 5, 8, 10, 12, 15, 20};

    std::cout << "\n=============================================\n";
    std::cout << "  CBS BENCHMARK RESULTS (ost003d.map)\n";
    std::cout << "=============================================\n";
    std::cout << " Agents\tTime (ms)\tResult\n";
    std::cout << "---------------------------------------------\n";

    for (int n : agent_counts) {
        std::vector<point> starts, goals;
        load_scenario_coords(scen_path, n, starts, goals);
        ASSERT_EQ(starts.size(), n) << "Failed to load " << n << " agents from scenario file";

        auto start_time = std::chrono::high_resolution_clock::now();
        auto solution = conflict_based_search(map_arr, starts, goals, n);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        
        EXPECT_TRUE(solution.has_value()) << "Solver failed to find a solution for " << n << " agents";

        std::cout << " " << n << "\t" << duration << " ms\t\t" 
                  << (solution.has_value() ? "Success" : "Failed") << "\n";
    }
    std::cout << "=============================================\n\n";
}
