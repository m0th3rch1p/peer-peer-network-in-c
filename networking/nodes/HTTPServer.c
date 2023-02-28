#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "HTTPServer.h"
#include "Route.h"

void register_routes(void (*route_function)(struct HTTPRequest *request, struct HTTPServer* server), char* uri, int no_of_methods, struct HTTPServer* server, ...);

void launch (struct Server* server) {

}

struct HTTPServer* http_server_constructor() {
    struct HTTPServer* server = (struct HTTPServer*)calloc(1, sizeof(struct HTTPServer));
    if (!server) {
        perror("Error allocating memory (HTTPServer)....");
        exit(1);
    }

    server->register_routes = register_routes;
    server->server = server_constructor(AF_INET, 0, SOCK_STREAM, INADDR_ANY, 8080, 255, launch);
    server->routes = dictionary_constructor(str_dict_cmp);
    return server;
}

void http_server_deconstructor(struct HTTPServer *server) {
    free(server);
}

void register_routes(void (*route_function)(struct HTTPRequest *request, struct HTTPServer* server), char* uri, int no_of_methods, struct HTTPServer* server, ...) {
    struct Route route;

    va_list methods;
    va_start(methods, no_of_methods);

    for (int i = 0; i < no_of_methods; i++) {
        route.methods[i] = va_arg(methods, int);
    }
    strcpy(route.uri, uri);

    route.route_function = route_function;
    server->routes->insert(uri, sizeof(char[strlen(uri)]), &route, sizeof(route), server->routes);
    
}
