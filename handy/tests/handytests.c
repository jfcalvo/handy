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

#include "handy.h"
#include "logging.h"

#define ALLOGRAPH_RECOGNIZE_FILE "../../data/allographs/test.bal"
#define COLLECTION_RECOGNIZE_FILE "../../data/collections/recog.bco"
#define COLLECTION_PROTOTYPE_FILE "../../data/collections/test.bco"
#define COLLECTION_UJI2 "../../data/collections/ujipen.uji2"

void allograph_memory_test(int num) {
    
    handy_allograph *allograph = NULL;
    
    int i = 0;
    for(i = 0; i < num; i++) {                
        info("Loading allograph %d from disk to memory", i);
        allograph = handy_allograph_load(ALLOGRAPH_RECOGNIZE_FILE,
                                         HANDY_SERIALIZE_BINARY);                                         
        info("Free allograph %d from memory", i);
        handy_allograph_free(allograph);
    }
}

void collection_memory_test(int num) {
    
    handy_collection *collection = NULL;
    
    int i = 0;
    for(i = 0; i < num; i++) {
        info("Loading collection %d from disk to memory", i);
        collection = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                           HANDY_SERIALIZE_BINARY);
                                           
        info("Free collection %d from memory", i);
        handy_collection_free(collection);
    }
}

void allograph_save_test(int num) {
    
    handy_allograph *allograph = handy_allograph_load(ALLOGRAPH_RECOGNIZE_FILE,
                                                        HANDY_SERIALIZE_BINARY);
    
    int i = 0;
    for(i = 0; i < num; i++) {
        info("Saving allograph %d", i);
        handy_allograph_save(allograph, "/tmp/test.bal", HANDY_SERIALIZE_BINARY);
    }
}

void collection_save_test(int num) {
    
    handy_collection *collection = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                                         HANDY_SERIALIZE_BINARY);
    
    int i = 0;
    for(i = 0; i < num; i++) {
        info("Saving collection %d", i);
        handy_collection_save(collection, "/tmp/test.bco", HANDY_SERIALIZE_BINARY);
    }
}

/*
 * Calculates the distance between an allographs and all elements of
 * a Collection.
 */
void allograph_collection_distance() {

    handy_recognize_config recognize_config;
    
    // Loading Allograph
    handy_allograph* allograph = handy_allograph_load(ALLOGRAPH_RECOGNIZE_FILE,
                                                        HANDY_SERIALIZE_BINARY);                                                                                    
                                                        
    // Loading Collection
    handy_collection* collection = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                                        HANDY_SERIALIZE_BINARY);
    // Recognize configuration
    recognize_config.recognize = HANDY_RECOGNIZE_DTW;
    recognize_config.distance_config.c = 0.849999;
    recognize_config.distance_config.boundary = false;
    recognize_config.distance_config.monotonicity = true;
    recognize_config.distance_config.symmetry = true;
    recognize_config.distance_config.distance = HANDY_DISTANCE_DTW;

    // Recognize the Allograph
    handy_allograph_recognize(allograph, collection, &recognize_config);
}

void allograph_collection_combinations() {
    
    handy_distance_config  distance_config;
    handy_recognize_config recognize_config;
    
    int boundary, monotonicity, symmetry;
    
    // Loading Allograph
    handy_allograph* allograph = handy_allograph_load(ALLOGRAPH_RECOGNIZE_FILE,
                                                        HANDY_SERIALIZE_BINARY);                                                                                    
                                                        
    // Loading Collection
    handy_collection* collection = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                                        HANDY_SERIALIZE_BINARY);
    // Recognize configuration
    recognize_config.recognize = HANDY_RECOGNIZE_DTW;
    recognize_config.distance_config.c = 0.849999;
    recognize_config.distance_config.boundary = false;
    recognize_config.distance_config.monotonicity = true;
    recognize_config.distance_config.symmetry = true;
    recognize_config.distance_config.distance = HANDY_DISTANCE_DTW;
    
    // Distance Initial configuration
    distance_config.c = 0.0;
    distance_config.boundary = false;
    distance_config.monotonicity = false;
    distance_config.symmetry = false;
        
    // Recognize the Allograph with differents combinations
    for(distance_config.c = 0.0; distance_config.c < 1.0; distance_config.c += 0.01) {
        for(boundary = 0; boundary < 2; boundary++) {
            distance_config.boundary = !distance_config.boundary;            
            for(monotonicity = 0; monotonicity < 2; monotonicity++) {
                distance_config.monotonicity = !distance_config.monotonicity;
                for(symmetry = 0; symmetry < 2; symmetry++) {
                    distance_config.symmetry = !distance_config.symmetry;
                    // Set recognize config to Distance Configuration
                    recognize_config.distance_config.c = distance_config.c;
                    recognize_config.distance_config.boundary = distance_config.boundary;
                    recognize_config.distance_config.monotonicity = distance_config.monotonicity;
                    recognize_config.distance_config.symmetry = distance_config.symmetry;
                    
                    info("Recognizer config, c=%f, boundary=%s, monotonicity=%s, symmetry=%s",
                         distance_config.c,
                         distance_config.boundary == true ? "true": "false",
                         distance_config.monotonicity == true ? "true": "false",
                         distance_config.symmetry == true ? "true": "false");
                         
                    handy_allograph_recognize(allograph, collection, &recognize_config);
                }
            }
        }
    }
}

/*
 * Find the best recognizer configuration for a group of allographs
 * in a collection, using other collection like the prototype.
 */
void collection_to_collection() {
    
    handy_uint  success = 0;    
    handy_float success_percent = 0.0;
    handy_float best_success_percent = 0.0;    
    
    handy_distance_config  distance_config;
    handy_distance_config  best_distance_config;    
    handy_recognize_config recognize_config;
    
    handy_allograph* tmp_allograph = NULL;
    
    int boundary, monotonicity, symmetry, i;
            
    // Recognize configuration
    recognize_config.recognize = HANDY_RECOGNIZE_DTW;
    recognize_config.distance_config.distance = HANDY_DISTANCE_DTW;
    
    // Distance Initial configuration
    distance_config.c = 0.0;
    distance_config.boundary = false;
    distance_config.monotonicity = false;
    distance_config.symmetry = false;    
    
    // Loading Collection to recognize
    handy_collection* collection_recognize = handy_collection_load(COLLECTION_RECOGNIZE_FILE,
                                                                   HANDY_SERIALIZE_BINARY);
    
    // Loading Collection Prototype
    handy_collection* collection_prototype = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                                                   HANDY_SERIALIZE_BINARY);
                                                                   
    // Recognize the Allographs and search the best configuration
    // for the recognizer
    for(distance_config.c = 0.0; distance_config.c < 1.0; distance_config.c += 0.01) {
        for(boundary = 0; boundary < 2; boundary++) {
            distance_config.boundary = !distance_config.boundary;
            for(monotonicity = 0; monotonicity < 2; monotonicity++) {
                distance_config.monotonicity = !distance_config.monotonicity;
                for(symmetry = 0; symmetry < 2; symmetry++) {
                    distance_config.symmetry = !distance_config.symmetry;
                    success = 0;
                    for(i = 0; i < collection_recognize->num_allographs; i++) {
                        // Set recognize config to distance configuration
                        recognize_config.distance_config.c = distance_config.c;
                        recognize_config.distance_config.boundary = distance_config.boundary;
                        recognize_config.distance_config.monotonicity = distance_config.monotonicity;
                        recognize_config.distance_config.symmetry = distance_config.symmetry;
                        
                        // Allograph copy
                        tmp_allograph = handy_allograph_new();
                        handy_allograph_copy(tmp_allograph, &collection_recognize->allographs[i]);
                        // Recognize Allograph
                        handy_allograph_recognize(tmp_allograph, collection_prototype, &recognize_config);                                                
                        
                        if(strcmp(tmp_allograph->symbol, collection_recognize->allographs[i].symbol) == 0) {
                            success++;
                        }
                                                                    
                        // Free temporal allograph
                        handy_allograph_free(tmp_allograph);
                    }
                    
                    success_percent = ((handy_float) success * 100.0 ) / ((handy_float) collection_recognize->num_allographs);
                    
                    info("Recognizer config, c=%f, boundary=%s, monotonicity=%s, symmetry=%s, success=%f",
                         distance_config.c,
                         distance_config.boundary == true ? "true": "false",
                         distance_config.monotonicity == true ? "true": "false",
                         distance_config.symmetry == true ? "true": "false",
                         success_percent);
                         
                    // Best configuration
                    if(success_percent >= best_success_percent) {
                        best_success_percent = success_percent;                        
                        best_distance_config.c = distance_config.c;
                        best_distance_config.boundary = distance_config.boundary;
                        best_distance_config.monotonicity = distance_config.monotonicity;
                        best_distance_config.symmetry = distance_config.symmetry;
                        
                        info("Best recognizer config by now, c=%f, boundary=%s, monotonicity=%s, symmetry=%s, success=%f",
                             best_distance_config.c,
                             best_distance_config.boundary == true ? "true": "false",
                             best_distance_config.monotonicity == true ? "true": "false",
                             best_distance_config.symmetry == true ? "true": "false",
                             best_success_percent);                           
                    }
                }                                
            }
        }
    }
        
    info("Best recognizer config, c=%f, boundary=%s, monotonicity=%s, symmetry=%s, success=%f",
         best_distance_config.c,
         best_distance_config.boundary == true ? "true": "false",
         best_distance_config.monotonicity == true ? "true": "false",
         best_distance_config.symmetry == true ? "true": "false",
         best_success_percent);   
}

/*
 * Calculates and show the distance between all elements of a
 * Collection.
 */
void collection_distance() {
    
    handy_float distance;
    handy_distance_config config;
    
    // Set distance algorithm configuration        
    config.c = 0.849999;
    config.boundary = false;
    config.monotonicity = true;
    config.symmetry = true;
    config.distance = HANDY_DISTANCE_DTW;
    
    // Loading collection from disk
    handy_collection* collection = handy_collection_load(COLLECTION_PROTOTYPE_FILE,
                                                         HANDY_SERIALIZE_BINARY);            
    int i = 0, j = 0;
    for(i = 0; i < collection->num_allographs; i++) {
        for(j = 0; j < collection->num_allographs; j++) {
            distance = handy_allograph_distance(&collection->allographs[i], 
                                                &collection->allographs[j], 
                                                &config);
            info("Distance between allograph %s and allograph %s is: %f", 
                 collection->allographs[i].symbol,
                 collection->allographs[j].symbol,
                 distance);
        }
    }
}

handy_bool test_handy_point_angle(handy_point p1, handy_point p2, handy_float angle) {
    
    handy_float angle_result = handy_point_angle(p1, p2);
    
    if(angle == angle_result) {
        info("TEST_OK: Point (%f, %f) and point (%f, %f) has the correct angle: %f",
             p1.x, p1.y, p2.x, p2.y, angle);
        return true;
    } else {
        error("TEST_FAIL: Point (%f, %f) and point (%f , %f) has the angle %f instead of %f",
              p1.x, p1.y, p2.x, p2.y, angle_result, angle);
        return false;
    }
}

void tests_handy_point_angle() {
   
    handy_float angle;
    handy_point p1, p2;
        
    p1.x = 100.0; // p1.x don't changes
    p1.y = 120.0; // p1.y don't changes
   
    // Quadrant 1 Tests
    
    // Q1: Angle = 0    
    p2.x = 200.0;
    p2.y = 120.0;
    
    angle = 0.0;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Q1: Angle = 45    
    p2.x = 200.0; 
    p2.y =  20.0;
    
    angle = 45.0;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Q1: Angle = 90    
    p2.x = 100.0;
    p2.y =  20.0;
    
    angle = 90.0;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Quadrant 2 Tests
    
    // Q2: Angle = 135
    p2.x =  80.0;
    p2.y = 100.0;    
    
    angle = 135.0;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Q2: Angle = 180
    p2.x =  80.0;
    p2.y = 120.0;
    
    angle = 180.0;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Quadrant 3 Tests
    
    // Q3: Angle = 225
    p2.x =  80.0;
    p2.y = 140.0;
    
    angle = 225;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Q3: Angle = 270
    p2.x = 100.0;
    p2.y = 140.0;
    
    angle = 270;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Quadrant 4 Tests
    
    // Q4: Angle = 315
    p2.x = 120.0;
    p2.y = 140.0;
    
    angle = 315;
    
    test_handy_point_angle(p1, p2, angle);
    
    // Equal points: Angle = 0    
    p2.x = p1.x;
    p2.y = p1.y;
    
    angle = 0.0;
    
    test_handy_point_angle(p1, p2, angle);    
}

void test_handy_collection_uji2() {
    
    handy_collection* collection;
    
    collection = handy_collection_load(COLLECTION_UJI2, HANDY_SERIALIZE_UJI2);
    
    handy_collection_free(collection);    
}

void test_handy_collection_info() {
    
    handy_collection* collection;
    handy_collection_info* info;
    
    // Load collection
    collection = handy_collection_load(COLLECTION_UJI2, HANDY_SERIALIZE_UJI2);
    
    // Get and print Info
    info = handy_collection_get_info(collection);
    printf("Collection file: %s\n", COLLECTION_UJI2);
    printf("Collection number of allographs: %d.\n", info->num_allographs);
    printf("Collection number of unique allographs: %d.\n", info->num_unique_allographs);
 
    printf("Collection list of allograph symbols: ");
    unsigned int i;
    for(i = 0; i < info->num_allographs; i++) {
        printf("%s, ", info->list_allographs_symbols[i]);
    }
    printf("\n");
    
    printf("Collection list of unique allograph symbols: ");
    for(i = 0; i < info->num_unique_allographs; i++) {
        printf("%s, ", info->list_unique_allographs_symbols[i]);
    }
    printf("\n");
    
    handy_collection_free(collection);
}

int main(int argc, char **argv) {
	
    //allograph_memory_test(500000);
    //collection_memory_test(500000);
    //allograph_save_test(500000);
    //collection_save_test(500000);
    
    //allograph_collection_distance();
    //allograph_collection_combinations();
    //collection_distance();
    //collection_to_collection();
    
    //tests_handy_point_angle();
    //test_handy_collection_uji2();
    test_handy_collection_info();
	
	return 0;
}
