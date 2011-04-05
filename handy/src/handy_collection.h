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

#ifndef HANDY_COLLECTION_H
#define HANDY_COLLECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_types.h"
#include "handy_allograph.h"

typedef struct {
    handy_uint       num_allographs; // Number of allographs in the collection
    handy_allograph* allographs;     // Array with all the allographs of the collection
} handy_collection;

typedef struct {
    handy_uint  num_allographs;            // Total number of allographs, for collection [a,b,c,b,a] is 5
    handy_uint  num_unique_allographs;     // Total number of different allographs, for collection [a,b,c,b,a] is 3
    handy_symbol* list_allographs_symbols; // List of allograph symbols, for collection [a,b,c,b,a] is [a,b,c,b,a].
    handy_symbol* list_unique_allographs_symbols; // List of different allograph symbols, for collection [a,b,c,b,a] is [a,b,c].
} handy_collection_info;

handy_collection* handy_collection_new();
void              handy_collection_free(handy_collection* collection);
int               handy_collection_add_allograph(handy_collection* collection, handy_allograph* allograph);

// Collection Utils

handy_collection_info* handy_collection_get_info(handy_collection* collection);
void                   handy_collection_free_info(handy_collection_info* info);

#ifdef __cplusplus
}
#endif

#endif /* HANDY_COLLECTION_H */
