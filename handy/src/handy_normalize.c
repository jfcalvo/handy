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
#include <math.h>

#include "handy_consts.h"
#include "handy_types.h"
#include "handy_allograph.h"
#include "handy_distance.h"
#include "handy_normalize.h"

#include "logging.h"

int handy_allograph_normalize(handy_allograph* allograph, handy_normalize normalization) {
        
    if(allograph != NULL) {
        if(allograph->num_points > 0) {
            // Normalize All flag        
            if((normalization & HANDY_NORMALIZE_ALL) == HANDY_NORMALIZE_ALL) {
                info("Normalize All: activated");
                normalization = HANDY_NORMALIZE_REPEATED_POINTS |
                                HANDY_NORMALIZE_NUMBER_OF_POINTS |
                                HANDY_NORMALIZE_SLANT |
                                HANDY_NORMALIZE_TRANSLATE_POINTS |
                                HANDY_NORMALIZE_SIZE_POINTS;
            }
            
            // Normalize Repeated Points            
            if((normalization & HANDY_NORMALIZE_REPEATED_POINTS) == HANDY_NORMALIZE_REPEATED_POINTS) {
                info("Normalize Repeated Points: activated");
                handy_allograph_normalize_repeated_points(allograph);        
            }
            
            // Normalize Number of Points
            if((normalization & HANDY_NORMALIZE_NUMBER_OF_POINTS) == HANDY_NORMALIZE_NUMBER_OF_POINTS) {
                info("Normalize Number of Points: activated");
                handy_allograph_normalize_number_of_points(allograph);
            }
            
            // Normalize Slant
            if((normalization & HANDY_NORMALIZE_SLANT) == HANDY_NORMALIZE_SLANT) {
                info("Normalize Slant: activated");
                handy_allograph_normalize_slant(allograph);
            }
            
            // Normalize Translate Points
            if((normalization & HANDY_NORMALIZE_TRANSLATE_POINTS) == HANDY_NORMALIZE_TRANSLATE_POINTS) {
                info("Normalize Translate Points: activated");
                handy_allograph_normalize_translate_points(allograph);        
            }
            
            // Normalize Size Points
            if((normalization & HANDY_NORMALIZE_SIZE_POINTS) == HANDY_NORMALIZE_SIZE_POINTS) {
                info("Normalize Size Points: activated");
                handy_allograph_normalize_size_points(allograph);        
            }
        } else {
            warning("The number of points in the Allograph is zero, normalization has not effect");
        } 
    } else {
        warning("Allograph has not been initiated, normalization has not effect");
    }
    
    return HANDY_SUCCESS_CODE;
}

int handy_allograph_normalize_repeated_points(handy_allograph *allograph) {
    
    handy_point* points = NULL;
    
    int i, j;
    int new_num_points = 1;
    int repeated_points = 0;

    // Search for repeated points
    for(i = 1; i < allograph->num_points; i++) {
        for(j = 0; j < new_num_points; j++) {
            // If the points are repeated
            if(allograph->points[i].x == allograph->points[j].x &&
               allograph->points[i].y == allograph->points[j].y) {                
                break;
            }
        }

        if(j == new_num_points) {
            allograph->points[new_num_points++] = allograph->points[i];
        }
    }
    
    // Calculating number of repeated points
    repeated_points = allograph->num_points - new_num_points;
    
    // Reallocating points array in memory
    if(new_num_points < allograph->num_points) {
        points = (handy_point*) realloc(allograph->points, sizeof(handy_point) * new_num_points);
        if(points == NULL) {
            error("Reallocating memory space for new normalized repeated points");
            return 0;
        } else {
            // Saving new points data
            allograph->points = points;
            allograph->num_points = new_num_points;            
        }
    }
    
    info("Removed %d repeated elements, new allograph size is %d", repeated_points, allograph->num_points);
    
    return repeated_points;
}

int handy_allograph_normalize_number_of_points(handy_allograph *allograph) {    
    
    // Epsilon is the tolerance value for Vexter Reduction algorithm
    handy_float epsilon = 10.0;
    
    // Reserving new normalize list of points
    handy_point* points = (handy_point*) malloc(sizeof(handy_point) * allograph->num_points);
    if(points == NULL) {
        error("Allocating memory for new normalize list of points");
        return 0;
    }
            
    // Adding the first point of the original allograph to the new
    // normalized list of points
    points[0] = allograph->points[0];
    
    handy_uint start = 0;
    handy_uint new_points_pos = 0;
    
    // Vertex Reducing algorithm
    handy_uint i = 0;    
    for(i = 0; i < allograph->num_points; i++) {
        // If the vertex is out of the epsilon tolerance
        if(handy_point_distance(allograph->points[start], 
                                allograph->points[i], 
                                HANDY_DISTANCE_EUCLIDEAN) > epsilon) {
        
            new_points_pos++;
            points[new_points_pos] = allograph->points[i];
            start = i;
        }
    }
        
    // Reallocating the new normalize list of points
    handy_uint new_num_points = new_points_pos + 1;
    points = (handy_point*) realloc(points, sizeof(handy_point) * new_num_points);
    if(points == NULL) {
        error("Reallocating memory for new normalize list of points");
        return 0;
    }
    
    info("Normalized number of points from %d points to %d points", allograph->num_points, new_num_points);    

    // Removing old points list from memory
    free(allograph->points);
    // Saving new normalize list of points in the allograph
    allograph->num_points = new_num_points;
    allograph->points = points;
        
    // Returning the new number of points of the allograph
    return new_num_points;
}

/*
 * Find the closest value in an sorted array
 * returns the index or -1 in case of error.
 */
handy_int handy_float_array_find(handy_float value, handy_uint array_size, const handy_float* array) {
    
    handy_int closed_index_value = 0;
    handy_float current_distance = 0.0;    
    handy_float last_distance = 0.0;    
    
    if(array_size > 0) {        
        closed_index_value = 0;
        last_distance = fabsf(value - array[0]);    
        
        handy_uint i;
        for(i = 1; i < array_size; i++) {
            current_distance = fabsf(value - array[i]);
            if(current_distance < last_distance) {
                last_distance = current_distance;
                closed_index_value = i;
            }
        }
    
        return closed_index_value;
        
    } else {
        error("Finding closed value in an empty array");
        return -1;
    }
}

int handy_allograph_normalize_slant(handy_allograph *allograph) {
    
    const static handy_float angle_threshold = HANDY_NORMALIZE_ANGLE_THRESHOLD;
    const static handy_float chain_angles[9] = {  0.00, // n0, 0.00 or 360.00
                                                 26.57, // n1
                                                 45.00, // n2
                                                 63.43, // n3
                                                 90.00, // n4
                                                116.57, // n5
                                                135.00, // n6
                                                153.43, // n7
                                                360.00  // n8
                                               };
    
    handy_float slant_estimation = 0.0;
    handy_float slant_estimation_degrees = 0.0;
    handy_float points_angle;
    handy_int closed_index_value;
    handy_uint chain_counter[8] = {0};
    
    // Creating center point
    handy_point center_point = handy_allograph_get_center(allograph);    
    
    handy_uint i;
    for(i = 0; i < allograph->num_points; i++) {                
        // Get the points angle
        points_angle = handy_point_angle(center_point, allograph->points[i]);
                                         
        info("Angle between point(%f, %f) and point[%d](%f, %f): %f",
             center_point.x, center_point.y,
             i, allograph->points[i].x, allograph->points[i].y, 
             points_angle);         
                                        
        // Evaluating where the angle must be inside the chain
        // angle list
        closed_index_value = handy_float_array_find(points_angle, 9, chain_angles);    
        info("Closed value for %f is %f", points_angle, chain_angles[closed_index_value]);
                                
        if(closed_index_value == 7) {                        
            if(points_angle >= chain_angles[closed_index_value] - angle_threshold &&
               points_angle <= chain_angles[closed_index_value] + angle_threshold) {
                   
                info("Angle selected is %d", closed_index_value);
                chain_counter[closed_index_value] += 1;
            } else {
                info("Not angle selected, is very far from %f", chain_angles[closed_index_value]);
            }            
        // Special value 360 grades is the same as 0 grades
        } else if(closed_index_value == 8) {            
            if(points_angle >= chain_angles[closed_index_value] - angle_threshold &&
               points_angle <= chain_angles[closed_index_value]) {
                 
                info("angle selected is 0");   
                chain_counter[0] += 1;
            } else {
                info("Not angle selected, is very far from %f", chain_angles[closed_index_value]);
            }            
        } else {
            info("Angle selected is %d", closed_index_value);
            chain_counter[closed_index_value] += 1;
        }                    
    }
    
    // Calculating slant estimation    
    slant_estimation = atanf(((2.0*chain_counter[1] + 2.0*chain_counter[2] + chain_counter[3]) - 
                              (chain_counter[5] + 2.0*chain_counter[6] + 2.0*chain_counter[7])) /
                             ((chain_counter[1] + 2.0*chain_counter[2] + 2.0*chain_counter[3]) + 2.0*chain_counter[4] +
                              (2.0*chain_counter[5] + 2.0*chain_counter[6] + chain_counter[7])));    
                                      
    // Slant estimation to degrees
    slant_estimation_degrees = slant_estimation * 180.0/M_PI;
    
    info("Slant allograph estimation: %f", slant_estimation);
    info("Slant allograph in degrees: %f", slant_estimation_degrees);
    
    // Shear transformation
    if((slant_estimation_degrees > 15.0 && slant_estimation_degrees < 90.0) || 
       (slant_estimation_degrees > 90.0 && slant_estimation_degrees < 135.0)) {
        info("Performing slant normalization");
        for(i = 0; i < allograph->num_points; i++) {
            allograph->points[i].x += allograph->points[i].y * tanf(slant_estimation);
        }
    } else {
        info("Slant estimation is out of threshold: %f", slant_estimation);
    }
        
    return 0;
}

int handy_allograph_normalize_size_points(handy_allograph* allograph) {
        
    static handy_float normalize_w = HANDY_NORMALIZE_WIDTH;
    static handy_float normalize_h = HANDY_NORMALIZE_HEIGHT;
    
    handy_float result_w;
    handy_float result_h;
    
    // Calculating the width and height of the allograph
    handy_point small_point = handy_allograph_get_smallest(allograph);
    handy_point big_point = handy_allograph_get_biggest(allograph);
        
    handy_float allograph_w = (handy_float) big_point.x - small_point.x + 1.0f;
    handy_float allograph_h = (handy_float) big_point.y - small_point.y + 1.0f;
    
    info("Allographs smallest points: x: %f, y: %f", small_point.x, small_point.y);
    info("Allographs biggest points: x: %f, y: %f", big_point.x, big_point.y);
    info("Allograph width: %f, height: %f", allograph_w, allograph_h);
    
    // Normalizing Allograph Size
    result_w = normalize_w;
    result_h = (allograph_h * normalize_w) / allograph_w;
    
    if(result_h > normalize_h) {
        result_w = (allograph_w * normalize_h) / allograph_h;
        result_h = normalize_h;
    }
    
    // Normalizing Points based in new size
    int i;
    for(i = 0; i < allograph->num_points; i++) {
        allograph->points[i].x = (result_w * allograph->points[i].x) / allograph_w;
        allograph->points[i].y = (result_h * allograph->points[i].y) / allograph_h;
    }
    
    // Translating points
    handy_float translated_point;
    
    if(result_w == normalize_w) {
        translated_point = abs((normalize_h / 2) - (result_h / 2));
        for(i = 0; i < allograph->num_points; i++) {
            allograph->points[i].y += translated_point;
        }
    } else {
        translated_point = abs((normalize_w / 2) - (result_w / 2));
        for(i = 0; i < allograph->num_points; i++) {
            allograph->points[i].x += translated_point;
        }
    }
            
    info("Normalized size of %d points", allograph->num_points);
    
    return allograph->num_points;
}

int handy_allograph_normalize_translate_points(handy_allograph* allograph) {
    
    // Get smallest x and y coordinates
    handy_point small_point = handy_allograph_get_smallest(allograph);
        
    int i;
    for(i = 0; i < allograph->num_points; i++) {
        info("Original point[%d]: (%f, %f)", i, allograph->points[i].x, allograph->points[i].y); 
        allograph->points[i].x -= small_point.x;
        allograph->points[i].y -= small_point.y;        
        info("Translated point[%d]: (%f, %f)", i, allograph->points[i].x, allograph->points[i].y);         
    }
    
    info("Smallest x: %f, y: %f", small_point.x, small_point.y);
    info("Translated %d points of the allograph", allograph->num_points);
    
    for(i = 0; i < allograph->num_points; i++) {

    }
    
    return allograph->num_points;
}
