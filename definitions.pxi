from libcpp.list cimport list
from libcpp cimport bool

DEF SCALE = 2 #Scaling factor to be used throughout the project
              #for displaying the map
DEF W=1
DEF G=2
DEF B=4

cdef extern from *:
    """
    typedef struct {
        int x;
        int y;
    } point;

    bool operator==(point a, point b) {
        return (a.x == b.x)&&(a.y == b.y);
    }
    """
    ctypedef struct point:
        int x
        int y

ctypedef struct Constraint_t:
    #disallowing a certain agent at a certain time and position
    point v
    int t

ctypedef struct Conflict_t: #custom typedef for describing a Conflict
    list[int] agents
    point v #vertex at which the Conflict occurs
    int t #time-step at which the Conflict occurs

ctypedef struct Problem_t:
    #custom typedef for describing the objective to be met by one specific agent
    int map_id
    point start
    point goal
    list[Constraint_t] constraints #assumed to be in order of t(time-step)
