#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define ACK "HOLA"
#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000

int pserver_handshake(int *to_client);
int pclient_handshake(int *to_server);










int pserver_setup();
int pserver_connect(int from_client);

#endif
