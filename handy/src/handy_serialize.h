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

#ifndef HANDY_SERIALIZE_H
#define HANDY_SERIALIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "handy_allograph.h"

#define HANDY_SERIALIZE_UJI2_NORMALIZATION HANDY_NORMALIZE_SIZE_POINTS|HANDY_NORMALIZE_TRANSLATE_POINTS 

typedef enum {
    HANDY_SERIALIZE_YAML,
    HANDY_SERIALIZE_JSON,
    HANDY_SERIALIZE_UJI2,
    HANDY_SERIALIZE_BINARY
} handy_serialize;

// Allographs

handy_allograph*  handy_allograph_load(char* path, handy_serialize format);
handy_allograph*  handy_allograph_load_binary(char* path);
handy_allograph* _handy_allograph_load_binary(FILE* fp, char* path);

int  handy_allograph_save(handy_allograph* allograph, char* path, handy_serialize format);
int  handy_allograph_save_binary(handy_allograph* allograph, char* path);
int _handy_allograph_save_binary(FILE* fp, handy_allograph* allograph, char* path);

// Collections

handy_collection* handy_collection_load(char* path, handy_serialize format);
handy_collection* handy_collection_load_uji2(char* path);
handy_collection* handy_collection_load_binary(char* path);

int handy_collection_save(handy_collection* collection, char* path, handy_serialize format);
int handy_collection_save_binary(handy_collection* collection, char* path);

#ifdef __cplusplus
}
#endif

#endif /* HANDY_SERIALIZE_H */
