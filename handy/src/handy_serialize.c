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
#include <stdlib.h>
#include <string.h>

#include "handy_consts.h"
#include "handy_allograph.h"
#include "handy_collection.h"
#include "handy_normalize.h"
#include "handy_serialize.h"

#include "logging.h"

handy_allograph* handy_allograph_load(char* path, handy_serialize format) {
    
    switch(format) {        
        case HANDY_SERIALIZE_BINARY:        
            return handy_allograph_load_binary(path);
            break;
    }
}

handy_allograph* handy_allograph_load_binary(char* path) {
        
    handy_allograph* allograph;
    
    // Opening file
    FILE* fp;
    fp = fopen(path, "rb");
    if(fp == NULL) {
        error("Opening file %s to read allograph");
        return NULL;
    }
    
    // Loading Allograph
    allograph = _handy_allograph_load_binary(fp, path);
    // Closing File
    fclose(fp);
    
    return allograph;
}

handy_allograph* _handy_allograph_load_binary(FILE* fp, char* path) {
    
    handy_allograph* allograph;
        
    // Creating new allograph
    if((allograph = handy_allograph_new()) == NULL) {
        error("Creating a new allograph to store data readed from file %s", path);
        return NULL;
    }
    
    // Reading allograph letter
    if(fread(&allograph->symbol, sizeof(allograph->symbol), 1, fp) != 1) {
        error("Reading allograph symbol from file %s", path);
        return NULL;
    }
    
    // Reading allograph number of points
    if(fread(&allograph->num_points, sizeof(allograph->num_points), 1, fp) != 1) {
        error("Reading allograph number of points from file %s", path);
        return NULL;
    }
    
    // Reserving memory to store the points
    allograph->points = (handy_point*) malloc(sizeof(handy_point) * allograph->num_points);
    if(allograph->points == NULL) {
        error("Reserving memory for allograph points reading from file %s", path);
        return NULL;
    }
    
    // Reading allograph points
    if(fread(allograph->points, sizeof(handy_point), allograph->num_points, fp) != allograph->num_points) {
        error("Reading allograph points from file %s", path);
        return NULL;
    }

    info("Loaded allograph %s with %d points from file %s", allograph->symbol, allograph->num_points, path);    
    return allograph;    
}

int handy_allograph_save(handy_allograph* allograph, char* path, handy_serialize format) {
    
    if(allograph != NULL) {
        if(allograph->num_points > 0) {            
            switch(format) {
                case HANDY_SERIALIZE_BINARY:
                    return handy_allograph_save_binary(allograph, path);
                    break;
            }
        } else {
            warning("The number of points in the Allograph is zero, save to disk has not effect");
        }
    } else {
        warning("Allograph has not been initiated, save to disk has not effect");
    }
    
    return HANDY_ERROR_CODE;
}

int handy_allograph_save_binary(handy_allograph* allograph, char* path) {
    
    int result_code;
    
    // Opening file
    FILE* fp;
    fp = fopen(path, "wb");
    if(fp == NULL) {
        error("Opening file %s to save allograph", path);        
        return HANDY_ERROR_CODE;
    }
    
    // Saving Allograph
    result_code = _handy_allograph_save_binary(fp, allograph, path);
    // Closing File
    fclose(fp);
    
    return result_code;
}

int _handy_allograph_save_binary(FILE* fp, handy_allograph* allograph, char* path) {
            
    // Writing allograph letter
    if(fwrite(&allograph->symbol, sizeof(allograph->symbol), 1, fp) != 1) {
        error("Writing allograph letter information at file %s", path);        
        return HANDY_ERROR_CODE;
    }
    
    // Writing allograph number of points
    if(fwrite(&allograph->num_points, sizeof(allograph->num_points), 1, fp) != 1) {
        error("Writing allograph number of points at file %s", path);        
        return HANDY_ERROR_CODE;
    }
    
    // Writing allograph points
    if(fwrite(allograph->points, sizeof(handy_point), allograph->num_points, fp) != allograph->num_points) {
        error("Writing allograph points at file %s", path);
        return HANDY_ERROR_CODE;
    }

    info("Saved allograph with %d points at file %s (binary format)", allograph->num_points, path);
    return HANDY_SUCCESS_CODE;
}

handy_collection* handy_collection_load(char* path, handy_serialize format) {
    
    switch(format) {
        case HANDY_SERIALIZE_UJI2:
            return handy_collection_load_uji2(path);
            break;        
        case HANDY_SERIALIZE_BINARY:
            return handy_collection_load_binary(path);
            break;
    }
}

handy_collection* handy_collection_load_uji2(char* path) {
    
    handy_point point;
    handy_allograph* allograph;
    handy_collection* collection;
    
    handy_uint num_points = 0;
    
    // String buffers
    char line_buffer[HANDY_LINE_BUFFER];
    char points[HANDY_LINE_BUFFER];
    char symbol[HANDY_SYMBOL_LENGTH];
    
    info("Loading uji2 collection from %s", path);
    
    // Opening file
    FILE* fp;
    fp = fopen(path, "rb");
    if(fp == NULL) {
        error("Opening file %s to read collection");
        return NULL;
    }
    
    // Init Collection
    collection = handy_collection_new();
    if(collection == NULL) {
        error("Allocating memory for a new collection");
        fclose(fp);
        return NULL;
    }
    
    // Reading lines
    allograph = NULL;
    while(fgets(line_buffer, HANDY_LINE_BUFFER, fp) != NULL) {
        
        // Word command        
        if(sscanf(line_buffer, "WORD %s %*s", symbol) == 1) {
            
            if(allograph != NULL) {
                // Normalizing Allograph before add to collection
                handy_allograph_normalize(allograph, HANDY_SERIALIZE_UJI2_NORMALIZATION);
                // Adding allograph to collection
                handy_collection_add_allograph(collection, allograph);                                
            }
            
            // Free allograph to store a new one
            handy_allograph_free(allograph);
            // Creating new allograph     
            if((allograph = handy_allograph_new()) == NULL) {
                error("Creating a new allograph to store data readed from file %s", path);
                return NULL;
            }
            
            // Set symbol to allograph
            handy_allograph_set_symbol(allograph, symbol);
            
        // Points command
        } else if(sscanf(line_buffer, "  POINTS %u # %[^\t\n]", &num_points, points) == 2) {
            
            char *coord;
                                    
            int i = 0;
            for(i = 0; i < num_points; i++) {
                if(i == 0) {
                    coord = strtok(points, " ");
                } else {
                    coord = strtok(NULL, " ");
                }
                
                if(coord != NULL) {
                    point.x = (handy_float) atoi(coord);
                    coord = strtok(NULL, " ");
                    if(coord != NULL) {
                        point.y = (handy_float) atoi(coord);                        
                        handy_allograph_add_point(allograph, point);
                    }
                }                                                                
            }
        }
    }
    
    if(allograph != NULL) {
        // Normalizing Allograph before add to collection
        handy_allograph_normalize(allograph, HANDY_SERIALIZE_UJI2_NORMALIZATION);
        // Adding allograph to collection
        handy_collection_add_allograph(collection, allograph);                                
    }
    
    fclose(fp);
    info("Collection with %d allographs readed successfull from file %s", collection->num_allographs, path);
    return collection;
}

handy_collection* handy_collection_load_binary(char* path) {
    
    handy_allograph* allograph;    
    handy_collection* collection;
    handy_uint num_allographs;
    
    info("Loading binary collection from %s", path);
    
    // Opening file
    FILE* fp;
    fp = fopen(path, "rb");
    if(fp == NULL) {
        error("Opening file %s to read collection");
        return NULL;
    }
    
    // Init Collection
    collection = handy_collection_new();
    if(collection == NULL) {
        error("Allocating memory for a new collection");
        fclose(fp);
        return NULL;
    }    
    
    // Loading number of allographs
    if(fread(&num_allographs, sizeof(collection->num_allographs), 1, fp) != 1) {
        error("Reading collection number of allographs from file %s", path);
        fclose(fp);
        return NULL;
    }
        
    // Loading allographs
    unsigned int i = 0;
    for(i = 0; i < num_allographs; i++) {
        info("Loading allograph number %d of %d from collection", i, num_allographs);
        allograph = _handy_allograph_load_binary(fp, path);
        if(allograph == NULL) {
            error("Error loading allograph number %d from collection at file %s", i, path);
            fclose(fp);
            handy_collection_free(collection);
            return NULL;
        } else {
            info("Adding allograph number %d to collection", i);
            if(handy_collection_add_allograph(collection, allograph) == HANDY_ERROR_CODE) {
                error("Trying to add allograph number %d to collection", i);
                fclose(fp);
                handy_collection_free(collection);
                return NULL;
            } else {
                info("Allograph %s added to collection", allograph->symbol);
                info("Removing temporal allograph %s", allograph->symbol);
                handy_allograph_free(allograph);
            }
        }
    }
        
    fclose(fp);
    info("Collection with %d allographs readed successfull from file %s", collection->num_allographs, path);
    return collection;    
}

int handy_collection_save(handy_collection* collection, char* path, handy_serialize format) {
    
    if(collection != NULL) {
        if(collection->num_allographs > 0) {
            switch(format) {
                case HANDY_SERIALIZE_BINARY:
                    return handy_collection_save_binary(collection, path);
                    break;
            }
        } else {
            warning("The number of allographs in the Collection is zero, save to disk has not effect");
        }
    } else {
        warning("Collection has not been initiated, binary save to disk has not effect");
    }
    
    return HANDY_ERROR_CODE;
}

int handy_collection_save_binary(handy_collection* collection, char* path) {
    
    // Opening file
    FILE* fp;
    fp = fopen(path, "wb");
    if(fp == NULL) {
        error("Opening file %s to save collection", path);
        return HANDY_ERROR_CODE;
    }
    
    // Saving number of allographs in the collection
    if(fwrite(&collection->num_allographs, sizeof(collection->num_allographs), 1, fp) != 1) {
        error("Writing number of allographs in the collection, at file %s", path);
        fclose(fp);
        return HANDY_ERROR_CODE;
    }
    
    // Saving Allographs
    unsigned int i = 0;
    for(i = 0; i < collection->num_allographs; i++) {
        info("Writing allograph number %d from collection", i);
        if(_handy_allograph_save_binary(fp, &collection->allographs[i], path) == HANDY_ERROR_CODE) {
            error("Error writing allograph number %d from collection at file %s", i, path);
            fclose(fp);
            return HANDY_ERROR_CODE;
        }
    }
    
    // Closing file
    fclose(fp);
    info("Saved Collection with %d allographs at file %s", collection->num_allographs, path);
    return HANDY_SUCCESS_CODE;
}
