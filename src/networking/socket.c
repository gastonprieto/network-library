/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#include "socket.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

t_socket socket_connect_to(char* ip, int port) {
    t_socket sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    connect(sock, (struct sockaddr*) &addr, sizeof(addr));
    return sock;
}

void socket_send(t_socket sock, uint8_t type, void* data, void* (*serializer_element)(void *, uint32_t *) ) {
    uint32_t length;
    void* data_serialized = serializer_element(data, &length);
    t_nipc* nipc = nipc_create(type, length, data_serialized);
    void* stream = nipc_serialize(nipc);
    send(sock, stream, nipc_serialize_size(nipc), 0);
    
    nipc_destroy(nipc);
    free(data_serialized);
    free(stream);
}

t_socket socket_listen_in(char* ip, int port) {
    return 0;
}