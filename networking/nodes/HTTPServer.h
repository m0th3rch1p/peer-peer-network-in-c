#ifndef HTTPSERVER_DEF
#define HTTPSERVER_DEF

#include "server.h"
#include "../protocols/HttpRequest.h"
#include "../../dataStructures/dictionaries_tree/dictionary.h"

struct HTTPServer {
    struct Server *server;
    struct Dictionary *routes;

    void (*launch)(struct HTTPServer *server);
    void (*register_routes)(void (*route_function)(struct HTTPRequest *request, struct HTTPServer* server), char* uri, struct HTTPServer* server, int no_of_methods, ...);
};

struct HTTPServer* http_server_constructor();
void http_server_deconstructor(struct HTTPServer *server);

char* render_template (int no_of_files, ...);
#endif