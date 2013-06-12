/* 
 * File:   serializers.h
 * Author: shinichi
 *
 * Created on June 12, 2013, 5:47 AM
 */

#ifndef SERIALIZERS_H
#define	SERIALIZERS_H

#include <stdint.h>

/* Serializer String */
char* string_serializer(char* aString, uint32_t* size);
char* string_unserialize(char* aString);
/* Fin Serializer String */

#endif	/* SERIALIZERS_H */

