#define W 1
#define G 2
#define B 4
#include <list>

typedef struct {
    int x;
    int y;
} point;

typedef struct{
    point v;
    int t;
} Constraint_t;

typedef struct {
    point start;
    point goal;
    std::list<std::pair<int, Constraint_t>> constraints;
} Problem_t;
