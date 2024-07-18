#pragma once

#ifndef CONFLICTBASEDSEARCH_CONVERTMAP_H
#define CONFLICTBASEDSEARCH_CONVERTMAP_H

#include "definitions.h"
#include <opencv2/opencv.hpp>

std::optional<std::pair<std::vector<std::vector<MapPixel>>, cv::Mat>> convert_map(int);

#endif //CONFLICTBASEDSEARCH_CONVERTMAP_H
