#include "../include/ConflictTree.h"
#include "../include/AStar.h"
#include <set>

CTree_Node::CTree_Node(double cost) : cost(cost) {
}

bool CTree_Node_Comparator::operator() (const std::shared_ptr<CTree_Node>& n1, const std::shared_ptr<CTree_Node>& n2)
{
    return n1->cost > n2->cost; //since, top() in priority_queue returns the greatest element by default
}

/*********************************
 *  Conflict Tree Implementation *
 *********************************/

ConflictTree::ConflictTree(int number_of_agents, CTree_Node& given_root) : n_agents(number_of_agents)
{
    root = std::make_shared<CTree_Node>(given_root);
    pq.push(root);
}

void ConflictTree::add_node(const std::shared_ptr<CTree_Node>& p_parent, const CTree_Node& new_node)
{
    std::shared_ptr<CTree_Node> new_node_ptr = std::make_shared<CTree_Node>(new_node);
    p_parent->children_list.push_back(new_node_ptr);
    pq.push(new_node_ptr);
}

bool ConflictTree::is_empty()
{
    return pq.empty();
}

std::shared_ptr<CTree_Node> ConflictTree::best_node()
{
    std::shared_ptr<CTree_Node> best_node_p = pq.top();
    pq.pop();
    return best_node_p;
}

std::optional<Conflict_t> ConflictTree::validate(const std::shared_ptr<CTree_Node>& node)
{
    std::vector<std::list<point>::iterator> l_it_vec;
    for (auto &solution: node->solution)
        l_it_vec.push_back(solution.begin());

    bool end_of_solution = false;
    int time = 0;
    while (!end_of_solution) {
        end_of_solution = true;

        struct MultisetElement {
            int agent;
            point v;
        };
        struct MultisetComparator {
            static bool comp(const point& a, const point& b)
            {
                if (a.x == b.x) return a.y < b.y;
                else return a.x < b.x;
            }
            bool operator() (const MultisetElement& a, const MultisetElement& b) const
            {
                return comp(a.v, b.v);
            }
        };
        std::multiset<MultisetElement, MultisetComparator> point_set;
        for (int i = 0; i < static_cast<int>(l_it_vec.size()); i++)
            point_set.insert(MultisetElement{i, *l_it_vec[i]});

        for (const auto &multisetElement: point_set) {
            size_t count = point_set.count(multisetElement);
            if (count > 1) {
                Conflict_t conflict;
                auto range = point_set.equal_range(multisetElement);
                for (auto it = range.first; it != range.second; ++it)
                    conflict.agents.push_back(it->agent);
                conflict.constraint.v = multisetElement.v;
                conflict.constraint.t = time;
                return conflict;
            }
        }

        time++;
        for (int i=0; i<static_cast<int>(l_it_vec.size()); i++) {
            l_it_vec[i]++;
            if (l_it_vec[i] == node->solution[i].end()) {
                l_it_vec[i]--;
            } else end_of_solution = false;
        }
    }
    return {};
}

std::pair<double, Solution_t> call_low_level(const std::vector<std::vector<MapPixel>>& map, const std::vector<Problem_t>& problems_without_constraints, const std::list<std::pair<int, Constraint_t>>& constraints)
{
    double sic = 0;
    Solution_t solution;
    std::vector<Problem_t> problems = problems_without_constraints;
    for (const auto& pair: constraints) {
        problems[pair.first].constraints.push_back(pair.second);
    }
    for (const Problem_t& problem: problems) {
        double cost;
        std::list<point> path;
        std::tie(path, cost) = low_level(map, problem);
        sic += cost;
        solution.push_back(path);
    }
    return {sic, solution};
}

std::optional<Solution_t> conflict_based_search(
        const std::vector<std::vector<MapPixel>>& map, const std::vector<point>& starts, const std::vector<point>& goals, int n_agents
)
{
    std::vector<Problem_t> problemsWithoutConstraints;
    for (int i = 0; i < n_agents; ++i) {
        Problem_t problem;
        problem.start = starts[i];
        problem.goal = goals[i];
        problemsWithoutConstraints.push_back(problem);
    }

    double sic;
    Solution_t solution;
    std::tie(sic, solution) = call_low_level(map, problemsWithoutConstraints, {});
    CTree_Node root(sic);
    root.solution = solution;
    ConflictTree CTree(n_agents, root);
    while (!CTree.is_empty()) {
        std::shared_ptr<CTree_Node> best_node_p = CTree.best_node();
        std::optional<Conflict_t> conflict = ConflictTree::validate(best_node_p);
        if (!conflict.has_value()) return best_node_p->solution;
        for (int agent: conflict.value().agents) {
            auto new_constraints = best_node_p->constraints;
            new_constraints.emplace_back(agent, conflict.value().constraint);
            std::tie(sic, solution) = call_low_level(map, problemsWithoutConstraints, new_constraints);
            CTree_Node new_node(sic);
            new_node.constraints = new_constraints;
            new_node.solution = solution;

            if (true)
            //TODO: take into account the case where SIC is infinity
                CTree.add_node(best_node_p, new_node);
        }
    }
    return {};
}
