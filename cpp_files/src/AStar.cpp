#include "../include/AStar.h"
#include <cmath>
#include <queue>

const double sqrt2 = sqrt(2);

bool operator==(point a, point b) {
    return (a.x == b.x)&&(a.y == b.y);
}

bool operator<(point a, point b) {
    return a.x < b.x; //only for creating a std::set of 'point's
}

typedef struct {
    double cost;
    int x;
    int y;
    int t;
} pq_node;

pq_node make_node(double cost, point p, int t) {
    pq_node ret;
    ret.cost = cost;
    ret.x = p.x;
    ret.y = p.y;
    ret.t = t;
    return ret;
}

bool operator<(const pq_node n1, const pq_node n2){
    return n1.cost > n2.cost; //since, top() in priority_queue returns the greatest element by default
}

class Cost
{
public:
    double val;

    Cost(double value) {
        val = value;
    }

    bool operator<(Cost c1) {
        if (this->val < 0) {
            return false;
        } else if (c1.val < 0) {
            return true;
        } else {
            return this->val < c1.val;
        }
    }

    bool operator==(Cost c1) {
        if (this->val<0 && c1.val<0) {
            return true;
        } else {
            return this->val == c1.val;
        }
    }

    Cost operator+(double other) {
        if (this->val<0) {
            return Cost(-1);
        } else {
            return Cost(this->val+other);
        }
    }
};


double cppfiles::AStarGraph::h(point v) {
    if (!h_calculated[v.y][v.x]) {
        h_vec[v.y][v.x] = hypot(v.x-goal.x, v.y-goal.y);
        h_calculated[v.y][v.x] = true;
    }
    return h_vec[v.y][v.x];
}

bool cppfiles::AStarGraph::not_constrained(point a, int t) {
    for (Constraint_t constraint: constraints) {
        if (constraint.t == t) {
            if (constraint.v.x == a.x && constraint.v.y == a.y){
                return false;
            }
        }
    }
    return true;
}

bool cppfiles::AStarGraph::is_reachable(point a, int t) {
    if (0 <= a.x && a.x < width && 0 <= a.y && a.y < height) {
        if (map_arr[a.y][a.x] == W) {
            if (not_constrained(a, t)) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::tuple<point, double, double>> cppfiles::AStarGraph::find_neighbours(point a, int t) {
    std::vector<std::tuple<point, double, double>> neighbours;//vector of ((point, time), cost)
    point left = {a.x-1, a.y};
    point right = {a.x+1, a.y};
    point up = {a.x, a.y-1};
    point down = {a.x, a.y+1};
    point up_right = {a.x+1, a.y-1};
    point up_left = {a.x-1, a.y-1};
    point down_right = {a.x+1, a.y+1};
    point down_left = {a.x-1, a.y+1};
    std::vector<point> temp;
    temp = {left, right, up, down, a};
    for (point each_point: temp) {
        if (is_reachable(each_point, t)) {
            neighbours.push_back({each_point, 1, h(each_point)-h(a)});
        }
    }
    temp = {up_right, up_left, down_right, down_left};
    for (point each_point: temp) {
        if (is_reachable(each_point, t)) {
            neighbours.push_back({each_point, sqrt2, h(each_point)-h(a)});
        }
    }
    return neighbours;
}

std::pair<std::list<point>, double> cppfiles::AStarGraph::optimal_path() {
    std::priority_queue<pq_node> q;
    std::vector<std::vector<Cost>> distance(height, std::vector<Cost>(width, Cost(-1)));
    std::vector<std::vector<point>> parent(height, std::vector<point>(width, {-1, -1}));
    std::vector<std::vector<bool>> processed(height, std::vector<bool>(width, false));

    int time = 0;

    distance[start.y][start.x].val = 0;
    parent[start.y][start.x] = start;

    q.push(make_node(0, start, time));
    std::vector<std::tuple<point, double, double>> neighbours;
    while (!q.empty()) {
        point a = {q.top().x, q.top().y};
        time = q.top().t;
        q.pop();
        if (processed[a.y][a.x]) continue;
        processed[a.y][a.x] = true;
        neighbours = find_neighbours(a, time);
        for (unsigned int i = 0; i<neighbours.size(); i++) {
            point b;
            double h_diff; //h(b-h(a)
            double d;
            std::tie(b, d, h_diff) = neighbours[i];
            if (distance[a.y][a.x] + d + h_diff < distance[b.y][b.x]) {//A* is much like Dijkstra's with d'(x,y)=d(x,y)+h(b)-h(a)
                distance[b.y][b.x] = distance[a.y][a.x] + d + h_diff;
                parent[b.y][b.x] = a;
                q.push(make_node(distance[b.y][b.x].val, b, time+1));
            }
        }
    }
    point cur_vertex = goal;
    std::list<point> path;
    path.push_back(start);
    while (!(parent[cur_vertex.y][cur_vertex.x] == cur_vertex)) {
        path.push_front(cur_vertex);
        cur_vertex = parent[cur_vertex.y][cur_vertex.x];
    }
    double cost = 0; //for calculating cost of path
    for (auto it2 = path.begin(), it1 = it2++;it2 != path.end(); it1++, it2++) {
        if((it1->x-it2->x) && (it1->y-it2->y)) { //i.e. diagonally adjacent
            cost += sqrt2;
        } else { //i.e. vertically or horizontally adjacent
            cost += 1;
        }
    }
    return {path, cost};
}

std::pair<std::list<std::pair<int, int>>, double> cppfiles::a_star(std::vector<std::vector<int>>& map_arr, int start_y, int start_x, int goal_y, int goal_x) {
    point start = {start_x, start_y};
    point goal = {goal_x, goal_y};
    Problem_t problem = {start, goal, std::list<std::pair<int, Constraint_t>>()};
    cppfiles::AStarGraph Cur_Graph(map_arr , problem);
    double cost;
    std::list<point> path;
    std::tie(path, cost) = Cur_Graph.optimal_path();
    std::list<std::pair<int, int>> return_path;
    for (point position: path) {
        return_path.push_back({position.y, position.x});
    }
    return std::make_pair(return_path, cost);
}

std::pair<std::list<point>, double> cppfiles::low_level(std::vector<std::vector<int>>& map_arr, Problem_t problem) {
    cppfiles::AStarGraph Cur_Graph(map_arr, problem);
    return Cur_Graph.optimal_path();
}