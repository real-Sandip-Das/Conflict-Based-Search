#include <fstream>
#include <iostream>
#include <sstream>
#include "../include/ConvertMap.h"
#include "../include/AStar.h"
#include "../include/ConflictTree.h"

void read_scenario(const std::string&, const std::string&, std::vector<point>&, std::vector<point>&, int&);

int main()
{
    std::string assets_path = "../../assets/";
    std::vector<std::string> map_vec = {"brc202d", "den520d", "ost003d"};
    int map_no;
    while (true) {
        std::cout << "Choose the map for loading scenario(collection of MAPF problems) in[1-3]: ";
        std::cin >> map_no;
        if ((map_no >= 1) && (map_no <= 3)) break;
        std::cout << "Choice must be between 1 and 3 (inclusive), try again." << std::endl;
    }
    std::string map_name = map_vec[map_no - 1];
    std::string map_path = assets_path + map_name + ".map";
    std::vector<std::vector<MapPixel>> map_arr = convert_map(map_path).value();

    /*
	std::vector<std::vector<MapPixel>> map_arr = {
	    {MapPixel::G, MapPixel::W, MapPixel::W, MapPixel::G},
	    {MapPixel::W, MapPixel::W, MapPixel::W, MapPixel::W},
	    {MapPixel::W, MapPixel::W, MapPixel::W, MapPixel::W},
	    {MapPixel::G, MapPixel::W, MapPixel::W, MapPixel::G}
	};
    std::vector<point> starts = {{1, 0}, {0, 1}};
    std::vector<point> goals = {{2, 3}, {3, 2}};
     */
    std::vector<point> starts, goals;
    int n_agents;
    read_scenario(assets_path, map_name, starts, goals, n_agents);
    auto solution = conflict_based_search(map_arr, starts, goals, n_agents);
    if (solution.has_value()) {
        std::ofstream json_file("../../visualizer/solution.json");
        json_file << "{\n";
        json_file << "  \"map\": [\n";
        for (size_t i = 0; i < map_arr.size(); ++i) {
            json_file << "    \"";
            for (size_t j = 0; j < map_arr[i].size(); ++j) {
                json_file << map_arr[i][j];
            }
            json_file << "\"" << (i == map_arr.size() - 1 ? "" : ",") << "\n";
        }
        json_file << "  ],\n";
        json_file << "  \"starts\": [";
        for (size_t i = 0; i < starts.size(); ++i) {
            json_file << "{\"x\":" << starts[i].x << ",\"y\":" << starts[i].y << "}" << (i == starts.size() - 1 ? "" : ",");
        }
        json_file << "],\n";
        json_file << "  \"goals\": [";
        for (size_t i = 0; i < goals.size(); ++i) {
            json_file << "{\"x\":" << goals[i].x << ",\"y\":" << goals[i].y << "}" << (i == goals.size() - 1 ? "" : ",");
        }
        json_file << "],\n";
        json_file << "  \"solution\": [\n";
        for (size_t i = 0; i < solution.value().size(); ++i) {
            json_file << "    [";
            auto& agent_solution = solution.value()[i];
            size_t pt_idx = 0;
            for (auto& path_point : agent_solution) {
                json_file << "{\"x\":" << path_point.x << ",\"y\":" << path_point.y << "}" << (pt_idx == agent_solution.size() - 1 ? "" : ",");
                pt_idx++;
            }
            json_file << "]" << (i == solution.value().size() - 1 ? "" : ",") << "\n";
        }
        json_file << "  ]\n";
        json_file << "}\n";
        json_file.close();
        std::cout << "Successfully written solution to solution.json\n";
    } else {
        std::cout << "No Solution for the given MAPF problem could be found" << std::endl;
    }
	return 0;
}

void read_scenario(const std::string& assets_path, const std::string& map_name, std::vector<point>& starts, std::vector<point>& goals, int& n_agents)
{
    std::cout << "Choose between Even and Random scenarios[e/R]: ";
    std::string evenOrRandom;
    std::cin >> evenOrRandom;
    std::string scenario_path_partial;
    if (std::toupper(evenOrRandom[0]) == 'E') {
        scenario_path_partial = assets_path + map_name + ".map-scen-even/scen-even/" + map_name + "-even-";
    } else {
        scenario_path_partial = assets_path + map_name + ".map-scen-random/scen-random/" + map_name + "-random-";
    }
    int scenario_num;
    while (true) {
        std::cout << "Choose serial number of scenario[1-25]: ";
        std::cin >> scenario_num;
        if ((scenario_num >= 1) && (scenario_num <= 25)) break;
        std::cout << "Serial Number must be between 1 and 25 (inclusive), try again." << std::endl;
    }
    std::string scenario_path = scenario_path_partial + std::to_string(scenario_num) + ".scen";

    std::cout << "Enter the number of problems to be loaded from the scenario (consecutive problems from the beginning of the file will be chosen): " << std::endl;
    std::cin >> n_agents;

    std::ifstream file(scenario_path);
    std::string line;
    std::getline(file, line); // line = "version 1"
    starts.clear();
    goals.clear();
    for (int i = 0; i < n_agents; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        int bucket_dummy;
        std::string map_name_dummy;
        int width_dummy, height_dummy, xs, ys, xg, yg;
        float path_length_dummy;
        iss >> bucket_dummy >> map_name_dummy >> width_dummy >> height_dummy >> xs >> ys >> xg >> yg >> path_length_dummy;
        starts.push_back({xs, ys});
        goals.push_back({xg, yg});
    }
}

