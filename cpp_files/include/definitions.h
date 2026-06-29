#pragma once

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define SCALE 2
#include <list>

using MapPixel = char;

typedef struct {
    int x;
    int y;
} point;

typedef struct{
    // Position v is unavailable at time t
    point v;
    int t;
} Constraint_t;

typedef struct {
    point start;
    point goal;
    std::list<Constraint_t> constraints;
} Problem_t;

#endif //DEFINITIONS_H
