# distutils: language = c++
# cython: language_level = 3

include "definitions.pxi"
import cv2 as cv
cimport cv2 as cv
import numpy as np
cimport numpy as np
from libc.stdio cimport *
from libcpp.string cimport string, to_string
from libcpp.vector cimport vector

cdef vector[string] map_dict = [b"brc202d.map", b"den520d.map", b"ost003d.map"]

cpdef convert_Map(int i): #returns a (2D Array, Map Image)
    #The 2D Array represents the map itself, but it is easier to process
    cdef string filename = b"assets/"+map_dict[i-1]
    cdef int linenum = 0
    cdef FILE* file
    cdef char[1024] line
    cdef int height
    cdef int width
    cdef int x, y #y along height, x along width

    cdef np.ndarray map_image 
    cdef np.ndarray map_arr
    file = fopen(filename.c_str(), "r")
    if file is NULL:
        print("Error reading file!")
        raise RuntimeError
    fgets(line, 1024, file)
    fgets(line, 1024, file); sscanf(line, "height %d", &height)
    fgets(line, 1024, file); sscanf(line, "width %d", &width)
    fgets(line, 1024, file)
    width += 1
    map_image = np.zeros((height, width, 3), dtype=np.uint8)
    map_arr = np.zeros((height, width), dtype=np.uint8)
    y = 0
    while fgets(line, 1024, file) is not NULL:
        x = 0
        while(line[x] != b'\0'):
            #white = '.' this is defined as W
            #green = 'T' this is defined as G
            #black = '@' this is defined as B
            #... in definitions.pxi
            if(line[x] == b'.'):
                map_image[y, x] = (255, 255, 255)
                map_arr[y, x] = W
            elif(line[x] == b'T'):
                map_image[y, x] = (0, 255, 0)
                map_arr[y, x] = G
            elif(line[x] == b'@'):
                map_image[y, x] = (0, 0, 0)
                map_arr[y, x] = B
            x += 1
        y += 1
    map_image = cv.resize(map_image, (width*SCALE, height*SCALE))
    np.savez("assets/map"+to_string(i).decode()+".npz", map_image = map_image, map_arr = map_arr)