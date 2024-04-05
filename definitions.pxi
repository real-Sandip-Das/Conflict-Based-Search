# distutils: language = c++
# distutils: sources = cpp_files/include/definitions.h
from libcpp.list cimport list
from libcpp cimport bool

DEF SCALE = 2 #Scaling factor to be used throughout the project
              #for displaying the map

cdef extern from "cpp_files/include/definitions.h":
    ctypedef struct point:
        int x
        int y

ctypedef struct Problem_t:
    #custom typedef for describing the objective to be met by one specific agent
    int map_id
    point start
    point goal
    list[Constraint_t] constraints #assumed to be in order of t(time-step)
