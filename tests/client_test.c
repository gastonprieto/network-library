/* 
 * File:   client_test.c
 * Author: shinichi
 *
 * Created on May 16, 2013, 7:39 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "networking/socket.h"
/*
 * Cliente que se conecta a un servidor determinado y manda un mensaje
 */
int main(int argc, char** argv) {
    t_socket socket_to_send_data = socket_connect_to("127.0.0.1", 10000);
    
    char* _serializer_string(char* element, uint32_t* size) {
        *size = strlen(element);
        return strdup(element);
    }
    
    socket_send(socket_to_send_data, 1, "HOLA", (void*) _serializer_string);
    return EXIT_SUCCESS;
}

