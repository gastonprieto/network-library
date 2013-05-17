/* 
 * File:   nipc.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:35 AM
 */


#ifndef NIPC_H
#define	NIPC_H

#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t operation;
    uint32_t length;
    char* payload;
} t_nipc;

t_nipc* nipc_create(uint8_t operation, uint32_t length, char* payload);
void    nipc_destroy(t_nipc*);
void*   serialize_nipc(t_nipc*);
size_t  serialize_size_stream(t_nipc*);

#endif	/* NIPC_H */

