#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "HTTPServer.h"
#include "Route.h"

#include "../../system/ThreadPool.h"

struct ClientServer {
    int client;
    struct HTTPServer* server;
};

void register_routes(void (*route_function)(struct HTTPRequest *request, struct HTTPServer* server), char* uri, struct HTTPServer* server, int no_of_methods,  ...);
void* handler(void* arg);
void launch(struct HTTPServer *server);
char* generate_response (char* buffer);
char* read_file (char* file_path);

struct HTTPServer* http_server_constructor() {
    struct HTTPServer* server = (struct HTTPServer*)calloc(1, sizeof(struct HTTPServer));
    if (!server) {
        perror("Error allocating memory (HTTPServer)....");
        exit(1);
    }

    server->launch = launch;
    server->register_routes = register_routes;
    server->server = server_constructor(AF_INET, 0, SOCK_STREAM, INADDR_ANY, 8080, 255);
    server->routes = dictionary_constructor(str_dict_cmp);
    return server;
}

void http_server_deconstructor(struct HTTPServer *server) {
    free(server);
}

void register_routes(void (*route_function)(struct HTTPRequest *request, struct HTTPServer* server), char* uri, struct HTTPServer* server, int no_of_methods, ...) {
    struct Route route;
    printf("fuckshit: no_of_methods: %d\n", no_of_methods);
    
    if (no_of_methods > 0) {
        va_list methods;
        va_start(methods, no_of_methods);

        for (int i = 0; i < no_of_methods; i++) {
            route.methods[i] = va_arg(methods, int);
        }
    }

    route.uri = uri;
    route.route_function = route_function;
    server->routes->insert(uri, sizeof(char[strlen(uri)]), &route, sizeof(route), server->routes);
    
}

void launch(struct HTTPServer *server) {
    struct ThreadPool* thread_pool = thread_pool_constructor(20);

    struct sockaddr* sock_addr = (struct sockaddr*) &server->server->address; 
    socklen_t server_addr_len = (socklen_t) sizeof(server->server->address);
    while (1) {
        struct ClientServer* client_server = (struct ClientServer*)calloc(1, sizeof(struct ClientServer));
        if (!client_server) {
            perror("error allocating ClientServer....");
            exit(1);
        }

        client_server->client =  accept(server->server->socket, sock_addr, &server_addr_len);
        client_server->server = server;

        struct ThreadJob thread_job = thread_job_constructor(handler, client_server);
        thread_pool->add_work(thread_job, thread_pool);
        // thread_job_deconstructor(thread_job);
    }
}

void* handler (void* args) {
    struct ClientServer* client_server = (struct ClientServer*) args;

    char req_str[30000] = {0};
    read(client_server->client, req_str, 30000);

    struct HTTPRequest* request = http_request_constructor(req_str);
    
    char* uri = (char *)request->request_line->search("uri", sizeof(char[strlen("uri")]), request->request_line)->val;
    struct Route* route = (struct Route*)client_server->server->routes->search(uri, sizeof(char[strlen(uri)]), client_server->server->routes)->val;

    char* response = route->route_function(request, client_server->server);


    printf("Response from route function: %s\n", response);
    write(client_server->client, response, sizeof(char[strlen(response)]));

    free(response);
    http_request_deconstructor(request);
    free(args);

    close(client_server->client);
    return NULL;
}

char* render_template (int no_of_files, ...) {
    char path[30000] = { 0 };
    char* document = calloc(1, 1000000);

    va_list files;
    va_start(files, no_of_files);

    char* cursor = document;
    for (int i = 0; i < no_of_files; i++)
    {
        char* file_path = va_arg(files, char*);
        char* buffer = read_file(file_path);

        // Reallocate memory
        strcpy(cursor, buffer);
        cursor += strlen(buffer);

        free(buffer);
    }
    
    char* response = generate_response(document);

    free(document);
    return response;
}

char* generate_response (char* buffer){
    char* response_frame = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: %d\r\n\r\n"
    "%s";

    int response_size = strlen(response_frame) + sizeof(unsigned int) + sizeof(char[strlen(buffer)]);
    char* response = (char*)calloc(1, response_size);
    if (!response) {
        perror("error allocating memory for response");
        exit(1);
    }

    snprintf(response, response_size, response_frame, sizeof(char[strlen(buffer)]), buffer);
    return response;
}

// Caller frees memory
char* read_file (char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file......");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = calloc(1, fileSize + 1);
    if (!buffer) {
        perror("Error allocating buffer......");
        fclose(file);
        exit(1);
    }

    fread(buffer, 1, fileSize, file);
    fclose(file);
    return buffer;
}