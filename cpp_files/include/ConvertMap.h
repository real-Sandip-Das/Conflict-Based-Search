#pragma once

#ifndef CONFLICTBASEDSEARCH_CONVERTMAP_H
#define CONFLICTBASEDSEARCH_CONVERTMAP_H

#include "definitions.h"
#include <vector>
#include <string>
#include <optional>

std::optional<std::vector<std::vector<MapPixel>>> convert_map(std::string);

#endif //CONFLICTBASEDSEARCH_CONVERTMAP_H
