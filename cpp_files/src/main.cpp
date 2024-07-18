#include <opencv2/opencv.hpp>
#include "../include/AStar.h"
#include "../include/ConflictTree.h"
#include <iostream> //debug

using namespace cv;

// The purpose of this file is to debug the A* implementation

int main()
{
    /*
	const Mat image = imread("/home/anonub/Projects/Conflict-Based-Search/lena.png");
	if ( !image.data )
	{
		std::cout << "No Image data" << std::endl;
		return -1;
	}
	namedWindow("Lena Image", WINDOW_AUTOSIZE );
	imshow("Lena Image", image);
	waitKey(0);
	destroyAllWindows();
     */
	std::vector<std::vector<int>> map_arr = {
	    {G, W, W, G},
	    {W, W, W, W},
	    {W, W, W, W},
	    {G, W, W, G}
	};
    std::vector<point> starts = {{1, 0}, {0, 1}};
    std::vector<point> goals = {{2, 3}, {3, 2}};
    auto solution = conflict_based_search(map_arr, starts, goals, 2);
    if (solution.has_value()) {
        for (auto& agent_solution: solution.value()) {
            for (auto& path_point: agent_solution) {
                std::cout << "{" << path_point.x << ", " << path_point.y << "} ";
            }
            std::cout << "\n";
        }
    }
	return 0;
}