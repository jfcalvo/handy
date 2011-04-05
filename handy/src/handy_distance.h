/*
    handy
    Copyright (C) 2009-2010 José Francisco Calvo Moreno
    
    This library is free software: you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>    
    
    José Francisco Calvo Moreno
    josefranciscocalvo@gmail.com
*/

#ifndef HANDY_DISTANCE_H
#define HANDY_DISTANCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_types.h"
#include "handy_allograph.h"

typedef enum {
    HANDY_DISTANCE_DTW, // Dynamic Time Warping distance
    HANDY_DISTANCE_EUCLIDEAN    
} handy_distance;

typedef struct {
    handy_float c;
    handy_bool  boundary;
    handy_bool  monotonicity;
    handy_bool  symmetry;
    handy_distance distance;
} handy_distance_config;

// Allograph Distance

handy_float handy_allograph_distance(handy_allograph* a1, handy_allograph* a2, handy_distance_config* config);
handy_float handy_allograph_distance_dtw(handy_allograph* a1, handy_allograph* a2, handy_distance_config* config);

// Allograph Distance Utils

inline handy_bool _dtw_continuity_condition(handy_allograph* a1, handy_allograph* a2, handy_uint i, handy_uint j, handy_float c);

// Point Distance

handy_float handy_point_distance(handy_point p1, handy_point p2, handy_distance distance);
handy_float handy_point_distance_euclidean(handy_point p1, handy_point p2);

// Point Angle

handy_float handy_point_angle(handy_point p1, handy_point p2);

#ifdef __cplusplus
}
#endif

#endif /* HANDY_DISTANCE_H */
