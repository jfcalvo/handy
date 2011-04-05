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
#include <math.h>

#include "handy_allograph.h"
#include "handy_distance.h"

#include "logging.h"

// Allograph Distance Calculations

handy_float handy_allograph_distance(handy_allograph* a1, handy_allograph* a2, handy_distance_config* config) {

    // Allograph one assert
    if(a1 == NULL) {
        error("Allograph one has not been initiated, distance calculation aborted");
        return (handy_float) -1.0;        
    }
    
    // Allograph two assert
    if(a2 == NULL) {
        error("Allograph two has not been initiated, distance calculation aborted");
        return (handy_float) -1.0;
    }
    
    // There is no config assert because config can be NULL
    // and the default configuration will be selected
        
    // Distance method selection
    switch(config->distance) {
        case HANDY_DISTANCE_DTW:
            return handy_allograph_distance_dtw(a1, a2, config);
            break;
            
        default:
            error("Not distance method indicated at distance configuration, distance calculation aborted");
            return (handy_float) -1.0;
            break;
    }
}

handy_float handy_allograph_distance_dtw(handy_allograph* a1, handy_allograph* a2, handy_distance_config* config) {
        
    info("Calculating Dynamic Time Warping distance between allograph %s and allograph %s",
         a1->symbol, a2->symbol);
    
    // Default config values
    handy_bool boundary = false;
    handy_bool monotonicity = false;
    handy_bool symmetry = false;
                                
    // Variables
    handy_bool match;
    handy_uint i = 0, j = 0;
    handy_uint start_i = 0, start_j = 0;
    handy_uint number_of_matches = 0;
    handy_uint a1_num_points = a1->num_points;
    handy_uint a2_num_points = a2->num_points;
    handy_float distance = 0.0;
    handy_float temp_distance;
    handy_float smallest_distance;    
    
    /* 
     * Continuity condition:
     * If c=1, continuity condition has no effect (all points matchs)
     * if c=0, we are using linear matching instead of DTW 
     */
    handy_float c = 0.13;
        
    // Asigning new config values
    if(config != NULL) {
        c = config->c;
        boundary = config->boundary;
        monotonicity = config->monotonicity;
        symmetry = config->symmetry;
    }
    
    info("DTW Distance Config: c = %f, boundary = %s, monotonicity = %s, symmetry = %s",
         c, boundary == true ? "true": "false" , 
            monotonicity == true ? "true": "false", 
            symmetry == true ? "true": "false");
        
    /* 
     * Boundary Condition:
     * If boundary condition is activated, the firts and last points
     * of the allograph are matched and included in the distance
     * calculation.        
     */
    if(boundary == true) {        
        distance += handy_point_distance_euclidean(a1->points[0], a2->points[0]);
        distance += handy_point_distance_euclidean(a1->points[a1->num_points - 1],
                                                   a2->points[a2->num_points - 1]);
        start_i = 1;
        start_j = 1;
        a1_num_points--;
        a2_num_points--;
        
        number_of_matches += 2;
        
        info("Matches points: %d, %d (boundary condition: enabled)", 0, 0);
        info("Matches points: %d, %d (boundary condition: enabled)", 
             a1->num_points - 1, a2->num_points - 1);
    }
    
    /*
     * Monotonicity Condition:
     * If monotonicity condition is activated and if the ith point of the
     * first curve matches with the jth point of the second curve, it is
     * not possible for for any point of the firts curve with index > i
     * to match with a point of the second curve with index < j, and for
     * any point on the firts curve with index < i to match with any point
     * on the second curve with index > j.
     */    
    if(monotonicity) {        
        // Monotonicity Condition: enabled        
        handy_uint next_j = start_j;
        for(i = start_j; i < a1_num_points; i++) {
            match = false;
            smallest_distance = HANDY_SMALLEST_DISTANCE;
            for(j = next_j; j < a2_num_points; j++) {
                if(_dtw_continuity_condition(a1, a2, i, j, c)) {
                    temp_distance = handy_point_distance_euclidean(a1->points[i], a2->points[j]);
                    if(temp_distance < smallest_distance) {
                        match = true;
                        next_j = j;
                        smallest_distance = temp_distance;                       
                    }
                }
            }
            
            if(match) {
                number_of_matches++;
                distance += smallest_distance;
            }
        }
        
        if(symmetry) {
            handy_uint next_i = start_i;
            for(j = start_j; j < a2_num_points; j++) {
                match = false;
                smallest_distance = HANDY_SMALLEST_DISTANCE;
                for(i = next_i; i < a1_num_points; i++) {
                    if(_dtw_continuity_condition(a2, a1, j, i, c)) {
                        temp_distance = handy_point_distance_euclidean(a2->points[j], a1->points[i]);
                        if(temp_distance < smallest_distance) {
                            match = true;
                            next_i = i;
                            smallest_distance = temp_distance;
                        }
                    }
                }
                
                if(match) {
                    number_of_matches++;
                    distance += smallest_distance;
                }                
            }
        }
                        
    } else {
        // Monotonicity Condition: disabled
        for(i = start_i; i < a1_num_points; i++) {
            match = false;
            smallest_distance = HANDY_SMALLEST_DISTANCE;
            for(j = start_j; j < a2_num_points; j++) {
                if(_dtw_continuity_condition(a1, a2, i, j, c)) {
                    temp_distance = handy_point_distance_euclidean(a1->points[i], a2->points[j]);
                    if(temp_distance < smallest_distance) {
                        match = true;
                        smallest_distance = temp_distance;
                    }
                }
            }
            
            if(match) {
                number_of_matches++;
                distance += smallest_distance;
            }
        }
        
        if(symmetry) {
            for(j = start_j; j < a2_num_points; j++) {
                match = false;
                smallest_distance = HANDY_SMALLEST_DISTANCE;
                for(i = start_i; i < a1_num_points; i++) {
                    if(_dtw_continuity_condition(a2, a1, j, i, c)) {
                        temp_distance = handy_point_distance_euclidean(a2->points[j], a1->points[i]);
                        if(temp_distance < smallest_distance) {
                            match = true;
                            smallest_distance = temp_distance;
                        }
                    }
                }
                
                if(match) {                    
                    number_of_matches++;
                    distance += smallest_distance;
                }
            }
        }
    }
    
    info("Number of points matches between allograph %s and allograph %s is: %d",
         a1->symbol, a2->symbol, number_of_matches);

    // Distance normalization     
    distance = ((handy_float) distance / (handy_float) number_of_matches);
    return distance;
}

inline handy_bool _dtw_continuity_condition(handy_allograph* a1, handy_allograph* a2, 
                                            handy_uint i, handy_uint j, handy_float c) {

    handy_float condition1, condition2;
    handy_float first_operator, second_operator;
            
    first_operator  = (handy_float) ((a2->num_points / a1->num_points) * i);
    second_operator = (handy_float) (c * a2->num_points);
    
    condition1 = (handy_float) (first_operator - second_operator);
    if(j >= condition1) {
        condition2 = (handy_float) (first_operator + second_operator);
        if(j <= condition2) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Point Distance Calculations

handy_float handy_point_distance(handy_point p1, handy_point p2, handy_distance distance) {
    switch(distance) {
        case HANDY_DISTANCE_EUCLIDEAN:
            return handy_point_distance_euclidean(p1, p2);
            break;
    }
}

handy_float handy_point_distance_euclidean(handy_point p1, handy_point p2) {
    return (handy_float) sqrtf(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)));
}

// Point Angle Calculations

handy_float handy_point_angle(handy_point p1, handy_point p2) {
    
    handy_float angle;
        
    /*
     * Calculating Angle    
     * We don't need to control zero division error because atanf
     * control float infinite exception values
     */
    angle = (handy_float) fabsf(atanf((p1.y - p2.y) / (p1.x - p2.x))) * 180.0/M_PI;
    
    /* 
     * Adding a value that depends of the quadrant of the second point
     * to calculate the real complete angle    
     */
    if(p2.x >= p1.x && p2.y <= p1.y) {
        // Quadrant 1
        if(p2.x == p1.x && p2.y == p1.y) {
            // p1 and p2 are the same points --> angle = 0
            return 0.0;
        }
    } else if(p2.x < p1.x && p2.y <= p1.y) {
        // Quadrant 2        
        angle = 180.0 - angle;
    } else if(p2.x <= p1.x && p2.y > p1.y) {
        // Quadrant 3        
        angle = 180.0 + angle;
    } else if(p2.x > p1.x && p2.y > p1.y) {
        // Quadrant 4        
        angle = 360.0 - angle;
    } else {
        error("No valid quadrant found for point (x: %f, y: %f)", p2.x, p2.y);
        return 0.0;
    }
    
    return angle;
}
