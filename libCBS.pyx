# distutils: language = c++
# cython: language_level = 3

include "definitions.pxi"
import A_Star
import numpy as np
from libcpp.vector cimport vector
from libcpp.deque cimport deque

ctypedef struct CTree_Node:
    Conflict_t conflict
    vector[list[int]] solutions
    int cost #Total Cost for each agent

cdef class ConflictTree:
    #Tree is being implemented as two vectors
    #one to store the Nodes, another two point to it's parent node
    cdef vector[CTree_Node] Nodes
    cdef vector[int] parent
    cdef int n_agents #number of agents
    def __init__(self, int number_of_agents):
        self.n_agents = number_of_agents
    cdef add_node(self, int parent, Conflict_t conflict, vector[deque[int]] solutions, int cost):
        cdef CTree_Node new_node = {conflict, solutions, cost}
        self.Nodes.push_back(new_node)
        self.Nodes.push_back(parent)
    cdef int get_parent(self, int index):
        return self.parent[index]
    cdef get_conflict(self, int index):
        return self.Nodes[index].conflict

cdef class MAPFProblem:
    cdef int map_id
    cdef int num_agents #number of agents
    cdef vector[int] start #starting points
    cdef vector[int] goal
    cdef ConflictTree CT

    def __init__(self, int map_id, int n_agents, vector[int] start, vector[int] goal):
        self.num_agents = n_agents
        self.start = start
        self.goal = goal
        self.CT = self.CT(self.num_agents)
    
    cdef (vector[list[int]], double) find_individual_paths(self):
        """
            vector[list[int]]: optimal path for individual agents themselves
            double: Sum of Individual Costs
        """
        cdef vector[list[int]] solutions
        for i in range(self.num_agents):
            cdef solution, cost = A_Star.a_star()
    
    cdef (vector[list[int]], double) solve(self):
        """
        Returns:
            vector[list[int]]: solution of MAPF problem as a vector of path of each agent
            double: Net-cost summed over all agents
        """
        cdef CTree_Node R
        R.conflict = {{}, {0, 0}, 0}
        R.solutions.resize()