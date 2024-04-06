#ifndef ASTAR_H
#define ASTAR_H

#include "definitions.h"
#include <vector>
#include <utility>
#include <list>

namespace cppfiles {
    class AStarGraph
    {
    private:
        std::vector<std::vector<int>>& map_arr;
        int height;
        int width;
        point start, goal;
        std::list<Constraint_t> constraints;

        //for storing values of teh heuristic function
        std::vector<std::vector<bool>> h_calculated;
        std::vector<std::vector<double>> h_vec;
        double h(point);
        bool not_constrained(point, int);
        bool is_reachable(point, int);
        std::vector<std::pair<point, double>> find_neighbours(point, int);
    public:
        std::pair<std::list<point>, double> optimal_path();

        AStarGraph(std::vector<std::vector<int>>& given_map_arr, Problem_t problem) : map_arr(given_map_arr) {
            map_arr = given_map_arr;
            height = map_arr.size();
            width = map_arr[0].size();
            start = problem.start;
            goal = problem.goal;
            h_calculated.resize(height, std::vector<bool>(width, false));
            h_vec.resize(height, std::vector<double>(width));
        }
    };

    std::pair<std::list<std::pair<int, int>>, double> a_star(std::vector<std::vector<int>>&, int, int, int, int); //for testing the A* implementation on individual agents
    std::pair<std::list<point>, double> low_level(std::vector<std::vector<int>>&, Problem_t); // for use in the Conflict Based Search Algorithm
}
#endif //ASTAR_H