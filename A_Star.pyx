# distutils: language = c++
# distutils: sources = cpp_files/src/AStar.cpp
# cython: language_level = 3

include "definitions.pxi"
import numpy as np
cimport numpy as np
import cv2 as cv
from libcpp.utility cimport pair
from libcpp.list cimport list
from libcpp.vector cimport vector

cdef extern from "cpp_files/include/AStar.h" namespace "cppfiles":
    pair[list[pair[int, int]], double] a_star(vector[vector[int]]& map_arr, int start_y, int start_x, int goal_y, int goal_x)
        # """
        # Returns:
        #     pair[list[pair[position.y, position.x]], cost]
        # """

cdef class AStarInstance:
    cdef vector[vector[int]] map_arr
    cdef int height, width
    cdef np.ndarray map_image
    def __init__(self, int map_id):
        cdef data = np.load("assets/map"+str(map_id)+".npz")
        self.map_image = data["map_image"]
        map_arr = data["map_arr"]
        self.height, self.width = map_arr.shape
        cdef int i, j
        self.map_arr.resize(self.height, vector[int](self.width))
        for i in range(self.height):
            for j in range(self.width):
                self.map_arr[i][j] = map_arr[i][j]
    
    cpdef double solve(self, bint display_path, int start_y, int start_x, int goal_y, int goal_x):
        cdef list[pair[int, int]] path
        cdef double cost
        print(a_star(self.map_arr, start_y, start_x, goal_y, goal_x)) #debug
        path, cost = a_star(self.map_arr, start_y, start_x, goal_y, goal_x)
        cdef int x, y
        if display_path:
            for (y, x) in path:
                cv.rectangle(self.map_image, (x*SCALE, y*SCALE), ((x+1)*SCALE-1, (y+1)*SCALE-1), color=(0, 0, 255))
            cv.imshow("Resultant Optimal Path", self.map_image)
            cv.waitKey(0)
            cv.destroyAllWindows()
        return cost