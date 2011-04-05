/*
    logging - A small logging library
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

/*
    Example of use:
    
    info("Point x: %d, Point y: %d", x_coord, y_coord);
    warning("Point x: %d is out of screen", x_coord);
    error("Point x is negative, only positive values are valid");
*/

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
    #define info(message, args...) verbose(INFO, __LINE__, __FILE__, message, ##args)
    #define warning(message, args...) verbose(WARNING, __LINE__, __FILE__, message, ##args)
    #define error(message, args...) verbose(ERROR, __LINE__, __FILE__, message, ##args)
#else
    #define info(message, args...)
    #define warning(message, args...)
    #define error(message, args...)
#endif

typedef enum {
    INFO = 0,
    WARNING = 1,
    ERROR = 2
} verbose_level;

void verbose(int level, int line_number, char* filename, char* message, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H */

