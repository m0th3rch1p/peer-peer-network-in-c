#ifndef ROUTE_DEF
#define ROUTE_DEF

#include "HTTPServer.h"
#include "../protocols/HttpRequest.h"

struct Route {
    int methods[9];
    char* uri;
    char* (*route_function)(struct HTTPRequest* request, struct HTTPServer* server);
};

// struct Route* route_constructor (char* (*route_function)(struct HTTPRequest* request, struct HTTPServer* server));

#endif