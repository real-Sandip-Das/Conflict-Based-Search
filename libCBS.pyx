# distutils: language = c++
# distutils: sources = libCBS.hpp
# cython: language_level = 3

include "definitions.pxi"
import A_Star
from libcpp.vector cimport vector
from libcpp.queue cimport priority_queue
from libcpp.stack cimport stack
from cython.operator cimport dereference as deref, postincrement as inc, postdecrement as dec

cdef extern from "libCBS.hpp":
    ctypedef struct CTree_Node:
        Conflict_t conflict
        vector[list[point]] solutions
        int cost #Total Cost summed over all agents
        CTree_Node* parent
        list[CTree_Node*] children_list

    cdef cppclass ConflictTree_cpp:
        ConflictTree(int, CTree_Node)
        ~ConflictTree()
        CTree_Node root
        int n_agents #number of agents
        priority_queue[CTree_Node] pq
        void add_node(CTree_Node*, CTree_Node)
        bool is_empty()
        CTree_Node* best_node()
        pair[bool, Conflict_t] validate(CTree_Node*)

cdef class ConflictTree:
    cdef ConflictTree_cpp* CT_cpp
    def __init__(self, int number_of_agents, CTree_Node given_root):
        CT_cpp = new ConflictTree_cpp(number_of_agents, given_root)
    def __dealloc__(self):
        #Deallocates memory occupied by the Nodes of the Tree using Depth-First Search
        del CT_cpp
    cdef add_node(self, CTree_Node* p_parent, CTree_Node new_node):
        """
        For adding a Node to the tree

        Args:
            p_parent: pointer to parent node
            new_node: child node(to be added) itself
        Returns:
            returns nothing
        """
        (*self.CT_cpp).add_node(p_parent, new_node)

    cdef bint is_empty(self):
        """
        This tree implementation stores a Priority Queue to return pointer to the Node having the least SIC
        (Sum of Individual Costs) on demand

        Returns:
            bint: whether the Priority Queue inside this class in empty
        """
        return <bint>((*self.CT_cpp).is_empty())

    cdef CTree_Node* best_node(self):
        """
        Returns:
            CTree_Node*: returns Unprocessed node in the Tree with the least SIC(Sum of Individual Costs), and
            removes(a.k.a. pop()) it from the priority_queue
        """
        return (*self.CT_cpp).best_node()

    cdef pair[bint, Conflict_t] validate(self, CTree_Node* node):
        """
        Checks whether the solution is valid(i.e. whether agents can follow it without collision,
        and if it isn't valid, then returns the first Conflict

        Args:
            node (CTree_Node): pointer to the Node
        Returns:
            (bint, Conflict_t): bint: whether valid/without Conflicts, Conflict_t: the first conflict, if there is any
        """
        return (*self.CT_cpp).validate(node)
                    #TODO: Modify each Node to have a list of Conflict_t(done in the cpp file)
        #TODO: Complete Implementing this validate function

cdef class MAPFProblem:
    cdef int map_id
    cdef int num_agents #number of agents
    cdef vector[int] start #starting points
    cdef vector[int] goal
    cdef ConflictTree CT

    def __init__(self, int map_id, int n_agents, vector[point] start, vector[point] goal):
        """
        Args:
            map_id (int):
            n_agents (int):
            start (vector[point]):
            goal (vector[point]):
        Returns:
            nothing
        """
        self.num_agents = n_agents
        self.start = start
        self.goal = goal
        self.CT = self.CT(self.num_agents)
    
    cdef (vector[list[point]], double) find_individual_paths(self):
        """
        Returns:
            (vector[list[point]], double):
                vector[list[int]]: optimal path for individual agents themselves, 
                double: Sum of Individual Costs
        """
        cdef vector[list[int]] solutions
        for i in range(self.num_agents):
            cdef solution, cost = A_Star.a_star()
    
    cdef (vector[list[point]], double) solve(self):
        """
        Returns:
            (vector[list[point]], double):
                vector[list[point]]: solution of MAPF problem as a vector of path of each agent
                double: Net-cost summed over all agents
        """
        cdef CTree_Node R
        R.solutions, R.cost = self.find_individual_paths()
        self.Nodes.push_back(R)
        self.parent.append(0)

        cdef int i
        for i