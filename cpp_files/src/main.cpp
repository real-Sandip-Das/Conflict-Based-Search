#include <opencv2/opencv.hpp>
#include "../include/ConvertMap.h"
#include "../include/AStar.h"
#include "../include/ConflictTree.h"

using namespace cv;

// The purpose of this file is to debug the A* implementation

void read_scenario(const std::string&, const std::string&, std::vector<point>&, std::vector<point>&, int&);

void visualize_solution(const Mat&, const std::string&, const Solution_t&);

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
	Mat map_image;
    std::vector<std::vector<MapPixel>> map_arr;
    std::tie(map_arr, map_image)  = convert_map(map_path).value();

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
        for (auto& agent_solution: solution.value()) {
            for (auto& path_point: agent_solution) {
                std::cout << "{" << path_point.x << ", " << path_point.y << "} ";
            }
            std::cout << "\n";
        }
        visualize_solution(map_image, "Conflict Based Search (visualization of solution)", solution.value());
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

void visualize_solution(const Mat& map_image_original, const std::string& window_name, const Solution_t& solution)
{
    namedWindow(window_name, WINDOW_NORMAL);

    // Map with full paths of individual agents in Blue
    cv::Mat map_with_paths;

    // Drawing each agent at different times
    cv::Mat visualization_window;
    bool end_of_solution = false;
    bool first_iteration = true;
    int time = 0;
    std::vector<std::list<point>::const_iterator> l_it_vec;
    while (true) {
        if (end_of_solution || first_iteration) {
            map_image_original.copyTo(map_with_paths);
            l_it_vec.clear();
            for (auto &agent_solution: solution)
                l_it_vec.push_back(agent_solution.begin());
            first_iteration = false;
        }

        map_with_paths.copyTo(visualization_window);
        for (auto l_it: l_it_vec) {
            int x = l_it->x, y = l_it->y;
            map_with_paths.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
            visualization_window.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
        }

        if (!visualization_window.data) {
            std::cerr << "No Image data" << std::endl;
            break;
        }
        imshow(window_name, visualization_window);
        if (waitKey(10) != -1) break;// 100 fps

        time++;
        end_of_solution = true;
        for (int i = 0; i < static_cast<int>(l_it_vec.size()); i++) {
            l_it_vec[i]++;
            if (l_it_vec[i] == solution[i].end()) {
                l_it_vec[i]--;
            } else end_of_solution = false;
        }
    }
    destroyAllWindows();
}
