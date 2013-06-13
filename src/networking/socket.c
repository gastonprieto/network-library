/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#include "socket.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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

void socket_multiplex(t_list* sockets) {
    struct epoll_event events[list_size(sockets)];
    int epollfd = epoll_create(list_size(sockets));
    
    void _add_elements(t_socket* socket) {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP;
        ev.data.fd = socket->socket;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, socket->socket, &ev);
    }

    list_iterate(sockets, (void*) _add_elements);
    
    int nfds = epoll_wait(epollfd, events, list_size(sockets), -1);
    for (int i = 0; i < nfds; ++i) {
        int sock_epoll = events[i].data.fd;
        uint32_t event = events[i].events;
        bool _search_by_socket(t_socket* socket) {
            return sock_epoll == socket->socket;
        }
        if (event & EPOLLRDHUP) {
            t_socket* sock_found = list_remove_by_condition(sockets, (void*) _search_by_socket);
            if (sock_found->handler_closed != NULL) {
                sock_found->handler_closed(sock_found);
            }
            free(sock_found);
        } else if (event & EPOLLIN) {
            t_socket* sock_found = list_find(sockets, (void*) _search_by_socket);
            sock_found->handler(sock_found);
        }
    }
    
    close(epollfd);
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