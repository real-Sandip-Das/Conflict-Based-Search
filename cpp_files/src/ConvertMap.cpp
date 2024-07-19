#include <../include/ConvertMap.h>
#include <fstream>
#include <cstdio>

std::optional<std::pair<std::vector<std::vector<MapPixel>>, cv::Mat>> convert_map(std::string map_path)
{
    std::ifstream file(map_path);
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return {};
    }

    std::string line;
    int height, width;
    std::getline(file, line);
    std::getline(file, line);
    std::sscanf(line.c_str(), "height %d", &height);
    std::getline(file, line);
    std::sscanf(line.c_str(), "width %d", &width);
    std::getline(file, line);
    cv::Mat map_image(height, width, CV_8UC3);
    std::vector<std::vector<MapPixel>> map_arr(height, std::vector<MapPixel>(width));

    int x, y = 0;
    while (std::getline(file, line)) {
        x = 0;
        for (char pixel: line) {
            if (pixel == '.') {
                map_image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                map_arr[y][x] = MapPixel::W;
            } else if (pixel == 'T') {
                map_image.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
                map_arr[y][x] = MapPixel::G;
            } else if (pixel == '@') {
                map_image.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                map_arr[y][x] = MapPixel::B;
            }
            x++;
        }
        y++;
    }

    file.close();
    return {{map_arr, map_image}};
}
