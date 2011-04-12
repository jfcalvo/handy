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

#ifndef HANDY_ALLOGRAPH_H
#define HANDY_ALLOGRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_types.h"
#include "handy_consts.h"

typedef struct {
	handy_float x, y, z; // Normaly z is used to save pen pressure level
} handy_point;

typedef struct {
    handy_symbol symbol;     // Symbol that represents the allograph
	handy_uint   num_points; // Number of points in the allograph
	handy_point* points;     // Array with all the points of the allograph
} handy_allograph;

handy_allograph* handy_allograph_new();
void             handy_allograph_free(handy_allograph* allograph);
int              handy_allograph_copy(handy_allograph* dst, handy_allograph* src);
int              handy_allograph_add_point(handy_allograph* allograph, handy_point point);
int              handy_allograph_set_symbol(handy_allograph* allograph, char* symbol);

// Allograph Utils

handy_point handy_allograph_get_center(handy_allograph* allograph);
handy_point handy_allograph_get_smallest(handy_allograph* allograph);
handy_point handy_allograph_get_biggest(handy_allograph* allograph);
  
// Point Utils

static inline handy_point handy_make_point(handy_float x, handy_float y) {
  return (handy_point) {x, y, 0.0f};
}

static inline handy_point handy_make_point_with_pressure(handy_float x, handy_float y, handy_float z) {
  return (handy_point) {x, y, z};
}
  
#ifdef __cplusplus
}
#endif

#endif /* HANDY_ALLOGRAPH_H */
