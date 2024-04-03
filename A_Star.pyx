# distutils: language = c++
# cython: language_level = 3
include "definitions.pxi"
import numpy as np
cimport numpy as np
import functools
from libc.math cimport hypot, sqrt
from libcpp.vector cimport vector
from libcpp.utility cimport pair
from libcpp.queue cimport priority_queue
from libcpp cimport bool


#for ordering in priority queue
cdef extern from *:
    """
    typedef struct {
        double cost;
        int x;
        int y;
        int t;
    } pq_node;

    bool operator<(const pq_node n1, const pq_node n2){
        return n1.cost < n2.cost;
    }
    """
    ctypedef struct pq_node:
        double cost
        int x
        int y
        int t

cdef pq_node make_node(double cost, point p, int t):
        cdef pq_node ret
        ret.cost = cost
        ret.x = p.x
        ret.y = p.y
        ret.t = t
    
@functools.total_ordering
cdef class Cost:
    cdef double val #negative means positive infinity
                 #non-negative value indicates itself
    def __init__(self, double value):
        self.val = value

    def __lt__(self, Cost other) -> bint:
        if self.val < 0:
            return False
        elif other.val < 0:
            return True
        else:
            return self.val < other.val

    def __eq__(self, Cost other) -> bint:
        if self.val<0 and other.val<0:
            return True
        else:
            return self.val == other.val
    
    def __add__(self, double other) -> Cost:
        if self.val<0:
            return Cost(-1)
        else:
            return Cost(self.val+other)

cdef class AStarGraph:
    cdef np.ndarray map_arr
    cdef int height, width
    cdef point start
    cdef point goal
    cdef list[Constraint_t] constraints

    #for storing values of the heuristic function
    cdef vector[vector[bint]] h_calculated
    cdef vector[vector[double]] __h_vec
    cdef readonly double sqrt2

    def __init__(self, np.ndarray map_image_arg, np.ndarray map_arr_arg, Problem_t Problem):
        self.map_image = map_image_arg
        self.map_arr = map_arr_arg
        self.height, self.width = <int>self.map_arr.shape[0],  <int>self.map_arr.shape[1]
        self.start = Problem.start
        self.goal = Problem.goal
        self.h_calculated.resize(self.height, vector[bint](self.width))
        self.__h_vec.resize(self.height, vector[double](self.width))
        self.sqrt2 = sqrt(2)
        self.constraints = Problem.constraints

    cdef double __h(self, point v):
        #the heuristic at use: Euclidean distance
        #Please note how Manhattan distance won't be a consistent heuristic in this case
        if not self.h_calculated[v.y][v.x]:
            self.__h_vec[v.y][v.x] = hypot(v.x-self.goal.x, v.y-self.goal.y)
        return self.__h_vec[v.y][v.x]

    cdef bint not_constrained(self, point a, int t):
        cdef Constraint_t constraint
        for constraint in self.constraints:
            if constraint.t == t:
                if constraint.v == a:
                    return False
        return True
    
    cdef bint is_reachable(self, point a, int t):
        if 0 <= a.x < self.width and 0 <= a.y < self.height:
            if self.map_arr[a.y, a.x] == W:
                if self.not_constrained(a, t):
                    return True
        return False
    
    cdef vector[(point, double)] find_neighbours(self, point a, int t):
        cdef vector[(point, double)] neighbours#vector of ((point, time), cost)
        cdef point left = {a.x-1, a.y}
        cdef point right = {a.x+1, a.y}
        cdef point up = {a.x, a.y-1}
        cdef point down = {a.x, a.y+1}
        cdef point up_right = {a.x+1, a.y-1}
        cdef point up_left = {a.x-1, a.y-1}
        cdef point down_right = {a.x+1, a.y+1}
        cdef point down_left = {a.x-1, a.y+1}
        cdef vector[point] temp 
        temp = {left, right, up, down, a}
        for each_point in temp:
            if self.is_reachable(each_point, t):
                neighbours.push_back((each_point, 1+self.__h(each_point)-self.__h(a)))
        temp = {up_right, up_left, down_right, down_left}
        for each_point in temp:
            if self.is_reachable(each_point, t):
                neighbours.push_back((each_point, self.sqrt2+self.__h(each_point)-self.__h(a)))
        return neighbours
    
    cdef (list[point], double) optimal_path(self):
        """
        #Implementation of A* Algorithm
        Returns:
            list[point]: Optimal path for given problem
            double: Cost of the solution
        """
        cdef priority_queue[pq_node] q
        cdef np.ndarray distance = np.full((self.height, self.width), Cost(-1), dtype='Cost')
        cdef np.ndarray parent = np.full((self.height, self.width), {-1,-1}, dtype='point')
        cdef np.ndarray processed = np.full((self.height, self.width), False, dtype='bint')

        cdef int time = 0

        distance[self.start.y, self.start.x].val = 0
        parent[self.start.y, self.start.x] = self.start

        q.push(make_node(0, self.start, time))
        cdef point a, b
        cdef unsigned int i
        cdef vector[(point, double)] neighbours
        cdef double w
        while not q.empty():
            a = {q.top().x, q.top().y}
            time = q.top().t
            q.pop()
            if processed[a.y, a.x]: continue
            processed[a.y, a.x] = True
            neighbours = self.find_neighbours(a, time)
            for i in range(neighbours.size()):
                b, w = neighbours[i]
                if distance[a.y, a.x]+w < distance[b.y, b.x]:
                    distance[b.y, b.x] = distance[a.y, a.x]+w
                    parent[b.y, b.x] = a
                    q.push(make_node(-distance[b.y, b.x].val, b, time+1))
        cdef point cur_vertex = self.goal
        cdef list[point] path
        while parent[cur_vertex.y, cur_vertex.x] != cur_vertex:
            path.push_front(cur_vertex)
            cur_vertex = parent[cur_vertex.y, cur_vertex.x]
        return path, distance[self.goal].val
        #return net cost and optimal path
    
cpdef (list[point], double) a_star(int map_id, Problem_t problem):
    """
    Returns:
        list[point]: Optimal path for given problem
        double: Cost of the solution
    """
    cdef data = np.load("assets/map"+str(map_id)+".npz")
    map_image = data["map_image"]
    map_arr = data["map_arr"]
    cdef AStarGraph G = AStarGraph(map_image, map_arr, problem)
    return G.optimal_path()