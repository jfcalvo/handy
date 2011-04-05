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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logging.h"

void verbose(int level, int line_number, char* filename, char* message, ...) {
        
    static char* level_str[] = {"INFO: ", "WARN: ", "ERROR: "};
    
    FILE* output = stdout;
    va_list ap;
    
    // Output selection by level
    switch(level) {
        case INFO:
            output = stdout;            
            break;
        case WARNING:
            output = stderr;
            break;
        case ERROR:
            output = stderr;
            // In case of error we show filename and line
            fprintf(output, "[%s, line:%d] ", filename, line_number);
            break;
    }
    
    // Debug message
    fprintf(output, "%s", level_str[level]);
    
    // Printing the rest of the message
    va_start(ap, message);
    vfprintf(output, message, ap);
    fprintf(output, "\n");
    va_end(ap);
    fflush(output);
    
    return;
} 

