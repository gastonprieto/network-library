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

t_nipc* nipc_create_only_header();
t_nipc* nipc_create(uint8_t operation, uint32_t length, char* payload);
void    nipc_destroy(t_nipc*);

size_t  nipc_size_header();

void*   nipc_serialize(t_nipc* nipc);
size_t  nipc_serialize_size(t_nipc* nipc);

#endif	/* NIPC_H */

