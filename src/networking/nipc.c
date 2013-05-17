/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#include "nipc.h"
#include <string.h>
#include <stdlib.h>

static size_t  serialize_size_header();

t_nipc* nipc_create(uint8_t operation, uint32_t length, char* payload) {
    t_nipc* nipc = calloc(1, sizeof(t_nipc));
    nipc->length = length;
    nipc->operation = operation;
    nipc->payload = payload;
    return nipc;
}

void nipc_destroy(t_nipc* nipc) {
    free(nipc);
}

void* serialize_nipc(t_nipc* nipc) {
    void* stream = malloc(serialize_size_stream(nipc));
    memcpy(stream, nipc, serialize_size_header(nipc));
    memcpy(stream + serialize_size_header(nipc), nipc->payload, nipc->length);
    return stream;
}

size_t serialize_size_stream(t_nipc* nipc) {
    return serialize_size_header(nipc) + nipc->length;
}

size_t serialize_size_header() {
    return sizeof(uint8_t) + sizeof(uint32_t);
}
