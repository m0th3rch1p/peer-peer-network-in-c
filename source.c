#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "./dataStructures/dictionaries_tree/dictionary.h"
#include "./networking/HttpRequest.h"
#include "./networking/server.h"

void retrieve_page (struct HTTPRequest* request, int socket);

void launch (struct Server* server) {
    char buffer[30000];
    long valRead;
    while (1) {
        printf("================= WAITING FOR CONNECTIONS ==============================\n");
        int address_length = sizeof(server->address);
        int new_sock = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&address_length);

        valRead = read(new_sock, buffer, 30000);
        struct HTTPRequest* request = http_request_constructor(buffer);

        retrieve_page(request, new_sock);
        close(new_sock);
    }
};

void retrieve_page (struct HTTPRequest* request, int socket) {
    char path[30000] = { 0 };

    char* url = strtok((char *)request->request_line->search("uri", sizeof(char[strlen("uri")]), request->request_line)->val, "?");
    char* vars = strtok(NULL, "\0");

    strcpy(path, "/home/z3r0/Desktop");
    if (!strcmp(url, "/test")) {
        strcat(path, url);
    } else strcat(path, "/index");

    strcat(path, ".html");

    FILE* f = fopen(path, "r");
    if (f == NULL) {
        perror("error opening file at path:");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long fSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(fSize + 1);
    
    int total_read = 0;
    while (total_read < fSize) {
        int bytes_read = fread(buffer + total_read, 1, fSize - total_read, f);
        if (bytes_read == 0) {
            perror("Error reading from file....");
            free(buffer);
            fclose(f);
            return;
        }
        total_read += bytes_read;
    }
    fclose(f);

    char response[30000] = {0};
    
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n", strlen(buffer));
    strcat(response, buffer);

    int total_written = 0;
    unsigned long response_size = strlen(response);
    
    while (response_size) {
        int bytes_written = write(socket, (unsigned char*)response + total_written, sizeof(char[strlen(response)]) - total_written);
        if (bytes_written <= 0) {
            perror("error writing to socket....");
            break;
        }
        total_written += bytes_written;
        response_size -= bytes_written;
    }
    free(buffer);
};

int main () {
    struct Server* server = server_constructor(AF_INET, 0, SOCK_STREAM, INADDR_ANY, 8080, 10, launch);
    server->launch(server);
}