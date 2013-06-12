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
#include <arpa/inet.h>

static addr_info* addr_create(char* ip, int port);

t_socket* socket_create() {
    t_socket* sock = calloc(1, sizeof(t_socket));
    sock->socket = socket(AF_INET, SOCK_STREAM, 0);
    return sock;
}

t_socket* socket_connect_to(char* ip, int port) {
    t_socket* sock = socket_create();
    addr_info* addr = addr_create(ip, port); 
    connect(sock->socket, (struct sockaddr*) addr, sizeof(addr_info));
    free(addr);
    
    return sock;
}

void socket_send(t_socket* sock, uint8_t type, void* data, void* (*serializer_element)(void *, uint32_t *) ) {
    uint32_t length;
    void* data_serialized = serializer_element(data, &length);
    t_nipc* nipc = nipc_create(type, length, data_serialized);
    void* stream = nipc_serialize(nipc);
    send(sock->socket, stream, nipc_serialize_size(nipc), 0);
    
    nipc_destroy(nipc);
    free(stream);
}

void* socket_recv(t_socket* socket, void* (*unserializer_element) (void*)) {
    t_nipc* nipc = nipc_create_only_header();
    recv(socket->socket, nipc, nipc_size_header(), MSG_WAITALL);
    nipc->payload = calloc(nipc->length, 1);
    recv(socket->socket, nipc->payload, nipc->length, MSG_WAITALL);
    
    void* element = unserializer_element(nipc->payload);
    
    nipc_destroy(nipc);
    return element;
}

t_socket* socket_listen_in(char* ip, int port) {
    t_socket* sock = socket_create();
    addr_info* addr = addr_create(ip, port);
    bind(sock->socket, (struct sockaddr*) addr, sizeof(addr_info));
    listen(sock->socket, 100);
    free(addr);
    return sock;
}

t_socket* socket_accept(t_socket* server_socket) {
    t_socket* sock = socket_create();
    sock->socket = accept(server_socket->socket, NULL, 0);
    return sock;
}

/***************************************
 * PRIVATE FUNCTIONS 
 ***************************************/

addr_info* addr_create(char* ip, int port) {
    addr_info* addr = calloc(1, sizeof(addr_info));
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    return addr;
}