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

#ifndef HANDY_TYPES
#define HANDY_TYPES

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "handy_consts.h"

typedef bool         handy_bool;
typedef float        handy_float;
typedef int          handy_int;
typedef unsigned int handy_uint;
typedef char         handy_symbol[HANDY_SYMBOL_LENGTH];

#ifdef __cplusplus
}
#endif

#endif /* HANDY_TYPES */
