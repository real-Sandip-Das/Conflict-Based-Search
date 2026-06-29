#include <../include/ConvertMap.h>
#include <fstream>
#include <cstdio>
#include <iostream>


std::optional<std::vector<std::vector<MapPixel>>> convert_map(std::string map_path)
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
    std::vector<std::vector<MapPixel>> map_arr(height, std::vector<MapPixel>(width));

    int x, y = 0;
    while (std::getline(file, line)) {
        x = 0;
        for (char pixel: line) {
            map_arr[y][x] = pixel;
            x++;
        }
        y++;
    }

    file.close();
    return map_arr;
}
