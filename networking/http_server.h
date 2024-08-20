#define MAX_BUFFER_SIZE 2048
#define PORT 3001
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"
#define NOT_FOUND "HTTP/1.1 404 Not Found\n"
#define MAX_METHOD_SIZE 16
#define MAX_URI_SIZE 256
#define MAX_VERSION_SIZE 16
#define get_response(method) ((strcmp(method, GET)==0) ? \
"HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, GET!" : \
(strcmp(method, POST)==0) ? \
"HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, POST!" : \
(strcmp(method, PUT)==0) ? \
"HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, PUT!" : \
(strcmp(method, DELETE)==0) ? \
"HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, DELETE!" : \
"HTTP/1.1 400 Bad Request\n" \
) 
#define http_method(uri, client_fd, method) ((strcmp(uri, "/") == 0) ? \
send(client_fd, get_response(method), strlen(get_response(method)), 0) : \
send(client_fd, NOT_FOUND, strlen(NOT_FOUND), 0) \
)