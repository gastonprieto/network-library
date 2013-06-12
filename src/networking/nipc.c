/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#include "nipc.h"
#include <string.h>
#include <stdlib.h>

size_t nipc_serialize_size(t_nipc* nipc);

t_nipc* nipc_create_only_header() {
    return calloc(1, sizeof(t_nipc));
}

t_nipc* nipc_create(uint8_t operation, uint32_t length, char* payload) {
    t_nipc* nipc = nipc_create_only_header();
    nipc->length = length;
    nipc->operation = operation;
    nipc->payload = payload;
    return nipc;
}

void nipc_destroy(t_nipc* nipc) {
    free(nipc->payload);
    free(nipc);
}

size_t nipc_size_header() {
    return sizeof(uint8_t) + sizeof(uint32_t);
}

void* nipc_serialize(t_nipc* nipc) {
    void* stream = malloc(nipc_serialize_size(nipc));
    memcpy(stream, nipc, nipc_size_header(nipc));
    memcpy(stream + nipc_size_header(nipc), nipc->payload, nipc->length);
    return stream;
}

size_t nipc_serialize_size(t_nipc* nipc) {
    return nipc_size_header(nipc) + nipc->length;
}
