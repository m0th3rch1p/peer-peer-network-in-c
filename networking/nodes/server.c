#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server.h"

struct Server* server_constructor(int domain, int protocol, int service, unsigned long interface, int port, int backlog) {
    struct Server* server = calloc(1, sizeof(struct Server));

    server->domain = domain;
    server->protocol = protocol;
    server->service = service;
    server->interface = interface;
    server->port = port;
    server->backlog = backlog;

    server->address.sin_family = domain;
    server->address.sin_port = htons(server->port);
    server->address.sin_addr.s_addr = htonl(interface);

    printf("domain: %d service: %d, protocol: %d\n", domain, service, protocol);
    server->socket = socket(domain, service, protocol);
    if (server->socket == -1) {
        perror("Failed to connect socket....");
        exit(1);
    }

    if(bind(server->socket, (struct sockaddr *)&server->address, sizeof(server->address))) {
        perror("Failed to bind server......");
        close(server->socket);
        exit(1);
    }

    if (listen(server->socket, server->backlog)) {
        perror("Failed to listen for connections.....");
        close(server->socket);
        exit(1);
    }

    printf("Server listening for connections....\n");
    return server;
}

void server_deconstructor(struct Server* server) {
    close(server->socket);
    free(server);
}