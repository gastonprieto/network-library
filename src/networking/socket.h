/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#ifndef SOCKET_H
#define	SOCKET_H

#include "nipc.h"
#include <netinet/in.h>
#include <commons/collections/list.h>

struct sock_struct {
    int socket;
    void (*handler) (struct sock_struct*);
    void (*handler_closed) (struct sock_struct*);
};
typedef struct sock_struct t_socket;

typedef struct sockaddr_in addr_info;

// Client Functions
t_socket* socket_connect_to(char* ip, int port);

// Server Functions
t_socket* socket_listen_in(char* ip, int port);
t_socket* socket_accept(t_socket* server_socket);
void socket_multiplex(t_list* sockets);


void* socket_recv(t_socket* socket, void* (*unserializer_element) (void*));
void  socket_send(t_socket* socket, uint8_t type, void* data,
                  void* (*serializer_element)(void *, uint32_t *) );


#endif	/* SOCKET_H */

