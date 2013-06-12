/* 
 * File:   serializers.c
 * Author: shinichi
 *
 * Created on June 12, 2013, 5:51 AM
 */

#include "serializers.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* string_serializer(char* aString, uint32_t* size) {
    *size = strlen(aString) + 1;
    return strdup(aString);
}

char* string_unserialize(char* aString) {
    return strdup(aString);
}
