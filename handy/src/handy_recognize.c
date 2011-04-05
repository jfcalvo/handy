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

#include <stdio.h>

#include "handy_types.h"
#include "handy_consts.h"
#include "handy_allograph.h"
#include "handy_collection.h"
#include "handy_distance.h"
#include "handy_recognize.h"

#include "logging.h"

int handy_allograph_recognize(handy_allograph* allograph, handy_collection* collection, handy_recognize_config* config) {
    
    // Allograph assert
    if(allograph == NULL) {
        error("Allograph to be recognized has not been initiated, recognition aborted");
        return HANDY_ERROR_CODE;
    } else {
        if(allograph->num_points == 0) {
            error("Allograph to be recognized has no points, recognition aborted");
            return HANDY_ERROR_CODE;
        }
    }
    
    // Collection assert
    if(collection == NULL) {        
        error("Collection used to recognize allograph has not been initiated, recognition aborted");
        return HANDY_ERROR_CODE;
    } else {
        if(collection->num_allographs == 0) {
            error("Collection used to recognize allograph has no allographs inside, recognition aborted");
            return HANDY_ERROR_CODE;
        }
    }
    
    // There is no config assert because config can be NULL
    // and the default configuration will be selected
        
    // Recognition method selection
    switch(config->recognize) {
        case HANDY_RECOGNIZE_DTW:
            return handy_allograph_recognize_dtw(allograph, collection, config);
            break;
        
        default:
            error("Not recognition method indicated at recognizer configuration, recognition aborted");
            return HANDY_ERROR_CODE;
            break;
    }    
}

int handy_allograph_recognize_dtw(handy_allograph* allograph, handy_collection* collection, handy_recognize_config* config) {

    handy_float temp_distance;
    handy_float distance = HANDY_SMALLEST_DISTANCE;                    
                
    handy_uint best_match = 0;
    handy_uint i = 0;
    for(i = 0; i < collection->num_allographs; i++) {
        temp_distance = handy_allograph_distance_dtw(allograph, &collection->allographs[i], &config->distance_config);
        info("Distance with element %s (number %d at collection): %f", 
             collection->allographs[i].symbol, i, temp_distance);
             
        if(temp_distance < distance) {
            distance = temp_distance;
            best_match = i;
        }
    }
    
    info("Allograph recognized!!!, asigned symbol is: %s, distance %f, element %d at collection",
         collection->allographs[best_match].symbol, distance,
         best_match);
                  
    handy_allograph_set_symbol(allograph, &collection->allographs[best_match].symbol[0]);
    
    return HANDY_SUCCESS_CODE;
}
