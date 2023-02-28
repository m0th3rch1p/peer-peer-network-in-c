#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "HttpRequest.h"
#include "../dataStructures/queues/queue.h"

void extract_header_fields (struct HTTPRequest *request, char* header_fields);

int method_select (char* method);

int str_cmp (void* data1, void* data2);

int str_dict_cmp (void* data1, void* data2);


struct HTTPRequest* http_request_constructor(char* request_str) {
    for (int i = 0; i < strlen(request_str) - 1; i++)
    {
        if (request_str[i] == '\n' && request_str[i + 1] == '\n') {
            request_str[i + 1] = '|';
        }
    }
    
    char* request_line = strtok(request_str, "\n");
    char* header_fields = strtok(NULL, "|");
    char* body = strtok(NULL, "|");

    struct HTTPRequest* request = (struct HTTPRequest *) calloc(1, sizeof(struct HTTPRequest));
    if (!request) {
        perror("Error allocating memory in req.....");
        exit(1);
    }

    char* method = strtok(request_line, " ");
    char* uri = strtok(NULL, " ");
    // strtok(NULL, " ");
    strtok(NULL, "/");
    char* http_version = strtok(NULL, "/");

    request->request_line = dictionary_constructor(str_dict_cmp);
    request->header_fields = dictionary_constructor(str_dict_cmp);
    request->body = dictionary_constructor(str_dict_cmp);

    request->request_line->insert("method", sizeof(char[strlen("method")]), method, sizeof(char[strlen(method)]), request->request_line);
    request->request_line->insert("uri", sizeof(char[strlen("uri")]), uri, sizeof(char[strlen(uri)]), request->request_line);
    request->request_line->insert("http_version", sizeof(char[strlen("http_version")]), http_version, sizeof(char[strlen(http_version)]), request->request_line);
    
    if (request->request_line->search("GET", sizeof(char[strlen("GET")]), request->request_line)) {
        extract_header_fields(request, (char *)request->request_line->search("uri", sizeof(char[strlen("uri")]), request->request_line)->val);
    }

    return request;
}

void http_request_deconstructor(struct HTTPRequest* request) {
    dictionary_destructor(request->request_line);
    dictionary_destructor(request->header_fields);
    dictionary_destructor(request->body);
    free(request);
}

void extract_header_fields (struct HTTPRequest *request, char* header_fields) {
    char fields[strlen(header_fields)];
    struct Queue *headers = queue_constructor();
    char* field = strtok(fields, "\n");
    while (field) {
        headers->enqueue((void*)field, sizeof(char[strlen(field)]), headers);
        field = strtok(NULL, "\n");
    }

    char* header = (char *)headers->dequeue(headers);
    while (header) {
        char* key = strtok(header, ":");
        char* value = strtok(NULL, "\0");

        if(value) {
            request->header_fields->insert(key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]), request->header_fields);
        }
    }
}   

int method_select (char* method) {
    if (!strcmp(method, "GET")) return GET;
    else if (!strcmp(method, "POST")) return POST;
    else if (!strcmp(method, "PUT")) return PUT;
    else if (!strcmp(method, "PATCH")) return PATCH;
    else if (!strcmp(method, "DELETE")) return DELETE;
    else if (!strcmp(method, "HEAD")) return HEAD;
    else if (!strcmp(method, "OPTIONS")) return OPTIONS;
    else if (!strcmp(method, "TRACE")) return TRACE;
    else return CONNECT;
}

int str_dict_cmp (void* data1, void* data2) {
    return str_cmp((char*)((struct DictionaryNode*)data1)->key, (char*)((struct DictionaryNode*)data2)->key);
}

int str_cmp (void* data1, void* data2) {
    int cmp = strcmp((char *)data1, (char*)data2);

    if (cmp < 0) return -1;
    else if (cmp > 0) return 1;
    return cmp;
}
