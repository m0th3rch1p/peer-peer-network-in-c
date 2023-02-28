#include "../dataStructures/dictionaries_tree/dictionary.h"

#ifndef HTTP_REQUEST_DEF
#define HTTP_REQUEST_DEF
enum HTTPMethods {
    GET,
    POST,
    PATCH,
    PUT,
    DELETE,
    OPTIONS,
    HEAD,
    TRACE,
    CONNECT
};

struct HTTPRequest {
    struct Dictionary* request_line;
    struct Dictionary* header_fields;
    struct Dictionary* body;

};
#endif

struct HTTPRequest* http_request_constructor(char* request);
void http_request_deconstructor(struct HTTPRequest* request);