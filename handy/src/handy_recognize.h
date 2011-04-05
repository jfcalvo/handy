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

#ifndef HANDY_RECOGNIZE_H
#define HANDY_RECOGNIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_allograph.h"
#include "handy_collection.h"
#include "handy_distance.h"

typedef enum {
    HANDY_RECOGNIZE_DTW
} handy_recognize;

typedef struct {
    handy_recognize recognize;
    handy_distance_config distance_config;
} handy_recognize_config;

int handy_allograph_recognize(handy_allograph* allograph, handy_collection* collection, handy_recognize_config* config);
int handy_allograph_recognize_dtw(handy_allograph* allograph, handy_collection* collection, handy_recognize_config* config);

#ifdef __cplusplus
}
#endif

#endif /* HANDY_RECOGNIZE_H */
