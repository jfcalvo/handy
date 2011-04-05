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

#ifndef HANDY_NORMALIZE_H
#define HANDY_NORMALIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_allograph.h"

typedef enum {
    HANDY_NORMALIZE_ALL = 1,
    HANDY_NORMALIZE_REPEATED_POINTS = 2,
    HANDY_NORMALIZE_NUMBER_OF_POINTS = 4,
    HANDY_NORMALIZE_SLANT = 8,
    HANDY_NORMALIZE_SIZE_POINTS = 16,
    HANDY_NORMALIZE_TRANSLATE_POINTS = 32
} handy_normalize;

int handy_allograph_normalize(handy_allograph* allograph, handy_normalize normalization);
int handy_allograph_normalize_repeated_points(handy_allograph *allograph);
int handy_allograph_normalize_number_of_points(handy_allograph *allograph);
int handy_allograph_normalize_slant(handy_allograph *allograph);
int handy_allograph_normalize_size_points(handy_allograph* allograph);
int handy_allograph_normalize_translate_points(handy_allograph* allograph);

#ifdef __cplusplus
}
#endif

#endif /* HANDY_NORMALIZE_H */
