#include <opencv2/opencv.hpp>
#include "../include/ConvertMap.h"
#include "../include/AStar.h"
#include "../include/ConflictTree.h"

using namespace cv;

// The purpose of this file is to debug the A* implementation

int main()
{
	Mat image;
    auto converted_map  = convert_map(3);
    image = converted_map.value().second;
	if ( !image.data )
	{
		std::cout << "No Image data" << std::endl;
		return -1;
	}
	namedWindow("Map Image", WINDOW_AUTOSIZE );
	imshow("Map Image", image);
	waitKey(0);
	destroyAllWindows();

	std::vector<std::vector<MapPixel>> map_arr = {
	    {MapPixel::G, MapPixel::W, MapPixel::W, MapPixel::G},
	    {MapPixel::W, MapPixel::W, MapPixel::W, MapPixel::W},
	    {MapPixel::W, MapPixel::W, MapPixel::W, MapPixel::W},
	    {MapPixel::G, MapPixel::W, MapPixel::W, MapPixel::G}
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