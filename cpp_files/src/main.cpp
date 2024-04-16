#include <opencv2/opencv.hpp>
#include "../include/AStar.h"
#include <iostream> //debug

// The purpose of this file is to debug the A* implementation

int main()
{
	std::vector<std::vector<int>> map_arr = {
	    {W, W, W, W},
	    {W, W, W, W},
	    {W, W, W, W},
	    {W, W, W, W}};
	std::pair<std::list<std::pair<int, int>>, double> a;
	a = cppfiles::a_star(map_arr, 0, 0, 3, 3);
	std::cout << a.second << std::endl;
	return 0;
}