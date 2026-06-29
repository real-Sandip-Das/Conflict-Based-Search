#pragma once

#ifndef CONFLICTBASEDSEARCH_CONVERTMAP_H
#define CONFLICTBASEDSEARCH_CONVERTMAP_H

#include "definitions.h"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Parses the map file because C++ has no native string split utilities.
 * @param map_path File path we hope actually exists and is readable.
 * @return 2D grid representation, or nullopt if the file format is garbage.
 */
std::optional<std::vector<std::vector<MapPixel>>> convert_map(std::string);

#endif //CONFLICTBASEDSEARCH_CONVERTMAP_H
