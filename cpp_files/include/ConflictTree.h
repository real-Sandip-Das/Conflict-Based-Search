#pragma once

#ifndef CONFLICTTREE_H
#define CONFLICTTREE_H

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <utility>
#include <memory>
#include <optional>
#include "definitions.h"

typedef std::vector<std::list<point>> Solution_t;

struct Conflict_t {
    std::list<int> agents;
    Constraint_t constraint;
};

struct CTree_Node {
    explicit CTree_Node(double cost);
    std::list<std::pair<int, Constraint_t>> constraints;
    Solution_t solution;
    const double cost;
    std::list<std::shared_ptr<CTree_Node>> children_list;
};
class CTree_Node_Comparator {
public:
    bool operator() (const std::shared_ptr<CTree_Node>&, const std::shared_ptr<CTree_Node>&);
};

/**
 * @brief High-level conflict tree tracker. Resolves collisions by spawning
 * child nodes until we run out of RAM.
 */
class ConflictTree
{
private:
    std::shared_ptr<CTree_Node> root;
    int n_agents;
    std::priority_queue<std::shared_ptr<CTree_Node>, std::vector<std::shared_ptr<CTree_Node>>, CTree_Node_Comparator> pq;
public:
    ConflictTree(int, CTree_Node&);
    ~ConflictTree() = default;
    void add_node(const std::shared_ptr<CTree_Node>&, const CTree_Node&);
    bool is_empty();
    std::shared_ptr<CTree_Node> best_node();
    static std::optional<Conflict_t> validate(const std::shared_ptr<CTree_Node>&);
};

/**
 * @brief The main MAPF solver. Prepare to wait if you pass more than 15 agents.
 * @param map The map grid.
 * @param starts Start locations of the agents.
 * @param goals Goal locations of the agents.
 * @param n_agents Number of agents (keep this small, seriously).
 * @return Solution paths or nullopt if we hit an infinite loop or impossible map.
 */
std::optional<Solution_t> conflict_based_search(const std::vector<std::vector<MapPixel>>& map, const std::vector<point>& starts, const std::vector<point>& goals, int n_agents);

#endif //CONFLICTTREE_H
