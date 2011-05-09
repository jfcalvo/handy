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

#include "logging.h"


handy_allograph* handy_allograph_new() {
	
	handy_allograph* allograph = (handy_allograph*) malloc(sizeof(handy_allograph));
	
	if(allograph == NULL) {
        return NULL;
    } else {
        // Init values
        allograph->symbol[0] = '\0';
        allograph->num_points = 0;
        allograph->points = NULL;
	
        return allograph;
    }
}

int handy_allograph_copy(handy_allograph* dst, handy_allograph* src) {
    
    info("Creating a copy of an allograph");
    
    if(src == NULL) {
        error("Source allograph to copy is not initiated");
        return HANDY_ERROR_CODE;
    }
    
    if(dst == NULL) {
        error("Destiny allograph to copy is not initiated");
        return HANDY_ERROR_CODE;
    }
    
    if(src->num_points > 0) {
        dst->points = (handy_point*) malloc(sizeof(handy_point) * src->num_points);
        if(dst->points == NULL) {
            error("Impossible to allocate memory for new points");
            return HANDY_ERROR_CODE;
        } else {            
            memcpy(dst->points, src->points, sizeof(handy_point) * src->num_points);
        }                
    } else {
        dst->points = NULL;
    }
    
    info("Copying other fields of the source allograph");    
    handy_allograph_set_symbol(dst, src->symbol);
    dst->num_points = src->num_points;
        
    return HANDY_SUCCESS_CODE;
}

void handy_allograph_free(handy_allograph* allograph) {
    info("Removing allograph from memory");
    if(allograph != NULL) {
        if(allograph->points != NULL) {
            info("Removing points from memory");
            free(allograph->points);
            allograph->points = NULL;
        }
        
        info("Removing allograph structure");
        free(allograph);
    } else {
        warning("Allograph has not been initiated, free not have effect");
    }
}

int handy_allograph_add_point(handy_allograph* allograph, handy_point point) {
	
	handy_point* points = NULL;
	
	allograph->num_points++;
	points = (handy_point*) realloc(allograph->points, sizeof(handy_point) * allograph->num_points);
	if(points == NULL) {
		error("Reallocating memory space for new points: %f, %f", point.x, point.y);		
		return HANDY_ERROR_CODE;
	} else {		
		allograph->points = points;
		allograph->points[allograph->num_points - 1] = point;
		info("Added point %f, %f to the allograph, number of points: %d", point.x, point.y, allograph->num_points);
		return HANDY_SUCCESS_CODE;
	}
}

int handy_allograph_set_symbol(handy_allograph* allograph, char* symbol) {
    
    info("Adding symbol %s to allograph", symbol);
    if(allograph == NULL) {
        error("Impossible to assign symbol %s to allograph, allograph not initiated", symbol);
        return HANDY_ERROR_CODE;
    } else {
        if(symbol == NULL) {
            error("Trying to set a null symbol at allograph");
            return HANDY_ERROR_CODE;
        } else {        
            strncpy(allograph->symbol, symbol, HANDY_SYMBOL_LENGTH);
            allograph->symbol[HANDY_SYMBOL_LENGTH - 1] = '\0';
            info("Symbol %s assigned to allograph", symbol);
            return HANDY_SUCCESS_CODE;
        }
    }
}

/* 
 * Returns for an allograph the numbers of points (always >= 0) 
 * that are contained inside a rect
 */
handy_uint  handy_allograph_points_contained(handy_allograph* allograph, handy_rect rect) {
  
  handy_point point;
  handy_uint  points_contained = 0;
  handy_float maximum_x, maximum_y;
  
  maximum_x = rect.origin.x + rect.size.width;
  maximum_y = rect.origin.y + rect.size.height;
  
  unsigned int i;
  for (i = 0; i < allograph->num_points; i++) {
    point.x = allograph->points[i].x;
    if (point.x >= rect.origin.x && point.x <= maximum_x) {
      point.y = allograph->points[i].y;
      if (point.y >= rect.origin.y && point.y <= maximum_y) {
        points_contained++;
      }
    }
  }
  
  return points_contained;
}

/*
 * Returns a handy point with the center point of the allograph
 */
handy_point handy_allograph_get_center(handy_allograph* allograph) {
    
    handy_point center, smallest, biggest;
    
    smallest = handy_allograph_get_smallest(allograph);
    biggest  = handy_allograph_get_biggest(allograph);
    
    center.x = (handy_float) ((biggest.x - smallest.x) / 2.0) + smallest.x;
    center.y = (handy_float) ((biggest.y - smallest.y) / 2.0) + smallest.y;
    
    info("Center point of allograph is (%f, %f)", center.x, center.y);
    
    return center;
}

/* 
 * Returns a handy point with the smallest x and y coordinates of an
 * allograph
 */
handy_point handy_allograph_get_smallest(handy_allograph* allograph) {
        
    handy_point point;
    
    if(allograph->num_points > 0) {
        point.x = allograph->points[0].x;
        point.y = allograph->points[0].y;
        
        int i;
        for(i = 1; i < allograph->num_points; i++) {
            // Get smallest x
            if(allograph->points[i].x < point.x) {
                point.x = allograph->points[i].x;
            }
            // Get smallest y
            if(allograph->points[i].y < point.y) {
                point.y = allograph->points[i].y;
            }
        }
    } else {        
        point.x = 0;
        point.y = 0;
        
        warning("Trying to get smallest point values for an empty allograph");
    }
    
    return point;
}

/*
 * Returns a handy point with the biggest x and y coordinates of an
 * allograph
 */
handy_point handy_allograph_get_biggest(handy_allograph* allograph) {
    
    handy_point point;
    
    if(allograph->num_points > 0) {
        point.x = allograph->points[0].x;
        point.y = allograph->points[0].y;
        
        int i;
        for(i = 1; i < allograph->num_points; i++) {
            // Get biggest x
            if(allograph->points[i].x > point.x) {
                point.x = allograph->points[i].x;
            }
            // Get biggest y
            if(allograph->points[i].y > point.y) {
                point.y = allograph->points[i].y;
            }
        }
    } else {
        point.x = 0;
        point.y = 0;
        
        warning("Trying to get biggest point values for an empty allograph");
    }
    
    return point;
}
