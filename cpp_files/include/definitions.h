#define W 1
#define G 2
#define B 4
#include <list>

typedef struct {
    int x;
    int y;
} point;

bool operator==(point a, point b) {
    return (a.x == b.x)&&(a.y == b.y);
}

bool operator<(point a, point b) {
    return a.x < b.x; //only for creating a std::set of 'point's
}

typedef struct{
    point v;
    int t;
} Constraint_t;

typedef struct {
    point start;
    point goal;
    std::vector<std::vector<int>>& map_arr;
    std::list<std::pair<int, Constraint_t>> constraints;
} Problem_t;

//TODO: bring other definitions from definitions.pxi