#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int protocol;
    int service;
    unsigned long interface;
    int port;
    int backlog;
    int socket;

    struct sockaddr_in address;
    void (*launch)(struct Server* server);
};

struct Server* server_constructor(int domain, int protocol, int service, unsigned long interface, int port, int backlog, void(*launch)(struct Server* server));
void server_deconstructor(struct Server* server);