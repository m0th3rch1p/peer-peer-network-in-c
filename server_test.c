#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "./networking/nodes/HTTPServer.h"

char* home (struct HTTPRequest* request, struct HTTPServer* server) {
    return render_template(2, "/home/z3r0/Desktop/demo/header.html", "/home/z3r0/Desktop/demo/index.html");
}

char* about (struct HTTPRequest* request, struct HTTPServer* server) {
    return render_template(2, "/home/z3r0/Desktop/demo/header.html", "/home/z3r0/Desktop/demo/about.html");
}

int main () {
    struct HTTPServer *server = (struct HTTPServer*)http_server_constructor();

    server->register_routes(home, "/", server, 0);
    server->register_routes(about, "/about", server, 0);
    server->launch(server);

    http_server_deconstructor(server);
    return 0;
}

