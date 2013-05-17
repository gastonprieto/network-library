/* 
 * File:   socket.h
 * Author: shinichi
 *
 * Created on April 14, 2013, 2:32 AM
 */

#include "nipc.h"

#ifndef SOCKET_H
#define	SOCKET_H

typedef int t_socket;

t_socket socket_connect_to(char* ip, int port);
t_socket socket_listen_in(char* ip, int port);


void     socket_send(t_socket sock, uint8_t type, void* data, 
                        void* (*serializer_element)(void *, uint32_t *) );


#endif	/* SOCKET_H */

