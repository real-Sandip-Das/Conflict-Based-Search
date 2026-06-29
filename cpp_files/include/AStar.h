#pragma once

#ifndef ASTAR_H
#define ASTAR_H

#include "definitions.h"
#include <vector>
#include <utility>
#include <list>

/**
 * @brief Standard A* search wrapper because single-agent pathfinding is
 * apparently too hard to write inline.
 */
class AStarGraph
{
private:
    const std::vector<std::vector<MapPixel>>& map_arr;
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
    std::vector<std::tuple<point, double, double>> find_neighbours(point, int);
public:
    /**
     * @brief Computes the optimal path. Pray that the heuristic is admissible.
     * @return Pair of path points and total cost (if it ever returns).
     */
    std::pair<std::list<point>, double> optimal_path();

    AStarGraph(const std::vector<std::vector<MapPixel>>& given_map_arr, const Problem_t& problem) :
        map_arr(given_map_arr), start(problem.start), goal(problem.goal)
    {
        height = map_arr.size();
        width = map_arr[0].size();
        constraints = problem.constraints;
        h_calculated.resize(height, std::vector<bool>(width, false));
        h_vec.resize(height, std::vector<double>(width));
    }
};

std::pair<std::list<std::pair<int, int>>, double> a_star(const std::vector<std::vector<MapPixel>>&, int, int, int, int); //for testing the A* implementation on individual agents
std::pair<std::list<point>, double> low_level(const std::vector<std::vector<MapPixel>>&, const Problem_t&); // for use in the Conflict Based Search Algorithm
#endif //ASTAR_H