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

#include <stdlib.h>
#include <string.h>

#include "handy_consts.h"
#include "handy_allograph.h"
#include "handy_collection.h"

#include "logging.h"

handy_collection* handy_collection_new() {
    
    handy_collection* collection = (handy_collection*) malloc(sizeof(handy_collection));
    
    if(collection == NULL) {
        return NULL;
    } else {
        // Init Values
        collection->num_allographs = 0;
        collection->allographs = NULL;
        
        return collection;
    }
}

void handy_collection_free(handy_collection* collection) {
    
    info("Removing collection from memory");
    if(collection != NULL) {
        if(collection->allographs != NULL) {
            info("Removing allographs from collection");
            int i = 0;
            unsigned int num_allographs = collection->num_allographs;
            for(i = 0; i < num_allographs; i++) {
                info("Removing points of allograph %d of %d from collection", i, num_allographs);
                free(collection->allographs[i].points);
            }
        }
        
        info("Removing collection allographs");
        free(collection->allographs);
        
        info("Removing collection structure");
        free(collection);
    }
}

int handy_collection_add_allograph(handy_collection* collection, handy_allograph* allograph) {
    
    handy_allograph* allographs = NULL;
    
    if(allograph->num_points > 0) {
        collection->num_allographs++;
        allographs = (handy_allograph*) realloc(collection->allographs, sizeof(handy_allograph) * collection->num_allographs);
        if(allographs == NULL) {
            error("Reallocating memory space for new allographs in the collection");
            return HANDY_ERROR_CODE;
        } else {            
            collection->allographs = allographs;
            handy_allograph_copy(&collection->allographs[collection->num_allographs -1], allograph);
            
            info("Added allograph to collection, number of allographs %d", collection->num_allographs);
            return HANDY_SUCCESS_CODE;
        }
    } else {
        warning("Trying to add an empty allograph to a collection");
        return HANDY_ERROR_CODE;
    }
}

handy_collection_info* handy_collection_get_info(handy_collection* collection) {

    handy_symbol* symbols = NULL;
    
    // Allocating collection info data structure
    handy_collection_info* info = (handy_collection_info*) malloc(sizeof(handy_collection_info));
    
    // Set initial info
    info->num_allographs = collection->num_allographs;
    info->num_unique_allographs = 0;
    info->list_allographs_symbols = (handy_symbol*) malloc(sizeof(handy_symbol) * collection->num_allographs);
    info->list_unique_allographs_symbols = NULL;
    
    // Creating an util bool array for repeated elements
    handy_bool* repeated_symbols = (handy_bool*) malloc(sizeof(handy_bool) * collection->num_allographs);
    memset(repeated_symbols, false, sizeof(handy_bool) * collection->num_allographs);
    
    unsigned int i, j;
    for(i = 0; i < collection->num_allographs; i++) {
        // Copy collection source symbol to symbol allographs list
        strcpy(info->list_allographs_symbols[i], collection->allographs[i].symbol);
        
        if(repeated_symbols[i] == false) {
            // Incrementing number of unique allographs
            info->num_unique_allographs++;
            
            // Copy collection source symbol to unique symbol allographs list
            symbols = (handy_symbol*) realloc(info->list_unique_allographs_symbols, sizeof(handy_symbol) * info->num_unique_allographs);
            if(symbols == NULL) {
                error("Reallocating memory space for list of unique allographs.");
                return NULL;                          
            } else {
                info->list_unique_allographs_symbols = symbols;
                strcpy(info->list_unique_allographs_symbols[info->num_unique_allographs - 1], collection->allographs[i].symbol);
            }
            
            // Fill all the same symbols with true
            repeated_symbols[i] = true;
            for(j = i + 1; j < collection->num_allographs; j++) {
                if(strcmp(collection->allographs[i].symbol, collection->allographs[j].symbol) == 0) {
                    repeated_symbols[j] = true;
                }
            }  
        }      
    }
    
    free(repeated_symbols);
    
    return info;
}
