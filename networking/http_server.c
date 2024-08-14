#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 2048
#define PORT 3001
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"
#define MAX_METHOD_SIZE 16
#define MAX_URI_SIZE 256
#define MAX_VERSION_SIZE 16

int is_valid_method(const char *method) {
    return strcmp(method, GET) == 0 ||
           strcmp(method, POST) == 0 ||
           strcmp(method, PUT) == 0 ||
           strcmp(method, DELETE) == 0;
}

int is_valid_uri(const char *uri) {
    if (uri[0] != '/') return 0;  // Ensure URI starts with '/'
    for (size_t i = 0; i < strlen(uri); i++) {
        if (!isalnum(uri[i]) && uri[i] != '/' && uri[i] != '_' && uri[i] != '-' && uri[i] != '.' && uri[i] != '?') {
            return 0;  // Allow alphanumeric characters, '/', '_', '-', '.', and '?'
        }
    }
    return 1;
}

void ensure_no_overflow(char *input, size_t max_length) {
    input[max_length - 1] = '\0';  // Ensure null termination to avoid buffer overflow
}

void http_get(char* uri, int client_fd) {
    if (strcmp(uri, "/") == 0) {
        char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, GET!";
        send(client_fd, response, strlen(response), 0);
    } else {
        char* response = "HTTP/1.1 404 Not Found\n";
        send(client_fd, response, strlen(response), 0);
    }
}

void http_post(char* uri, int client_fd) {
    if (strcmp(uri, "/") == 0) {
        char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, POST!";
        send(client_fd, response, strlen(response), 0);
    } else {
        char* response = "HTTP/1.1 404 Not Found\n";
        send(client_fd, response, strlen(response), 0);
    }
}

void http_put(char* uri, int client_fd) {
    if (strcmp(uri, "/") == 0) {
        char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, PUT!";
        send(client_fd, response, strlen(response), 0);
    } else {
        char* response = "HTTP/1.1 404 Not Found\n";
        send(client_fd, response, strlen(response), 0);
    }
}

void http_delete(char* uri, int client_fd) {
    if (strcmp(uri, "/") == 0) {
        char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, DELETE!";
        send(client_fd, response, strlen(response), 0);
    } else {
        char* response = "HTTP/1.1 404 Not Found\n";
        send(client_fd, response, strlen(response), 0);
    }
}

void http_error(char* method, int client_fd) {
    printf("Unknown HTTP method: %s\n", method);
    char* response = "HTTP/1.1 400 Bad Request\n";
    send(client_fd, response, strlen(response), 0);
}

void handle_request(char* method, char* uri, int client_fd) {
    if (strcmp(method, GET) == 0)
        http_get(uri, client_fd);
    else if (strcmp(method, POST) == 0)
        http_post(uri, client_fd);
    else if (strcmp(method, PUT) == 0)
        http_put(uri, client_fd);
    else if (strcmp(method, DELETE) == 0)
        http_delete(uri, client_fd);   
    else
        http_error(method, client_fd);
}

void parse_body(char* buffer) {
    // Find the Content-Length header
    char* content_length_ptr = strstr(buffer, "Content-Length: ");
    if (content_length_ptr != NULL) {
        int content_length = atoi(content_length_ptr + 16); // Skip past "Content-Length: "

        // Validate content length
        if (content_length < 0 || content_length >= MAX_BUFFER_SIZE) {
            printf("Invalid content length\n");
            return;
        }

        // Find the start of the body
        char* body_ptr = strstr(buffer, "\r\n\r\n");
        if (body_ptr != NULL) {
            body_ptr += 4; // Skip past "\r\n\r\n"

            // Read the rest of the body
            if (content_length > 0) {
                char body[MAX_BUFFER_SIZE] = {0};
                strncpy(body, body_ptr, strlen(body_ptr));
                ensure_no_overflow(body, MAX_BUFFER_SIZE);

                printf("Body: %s\n", body);     
            }  
        }
    }
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int address_len = sizeof(address);
    char buffer[MAX_BUFFER_SIZE];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr*)&address, address_len) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

        // Accept a client connection
        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&address_len)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Clear the buffer before use
        memset(buffer, 0, MAX_BUFFER_SIZE);

        // Read the client's request
        read(client_fd, buffer, MAX_BUFFER_SIZE - 1);
        ensure_no_overflow(buffer, MAX_BUFFER_SIZE);
        printf("%s\n", buffer);

        // Parse the HTTP method, the request URI, and the HTTP version
        char method[MAX_METHOD_SIZE], uri[MAX_URI_SIZE], version[MAX_VERSION_SIZE];
        sscanf(buffer, "%15s %255s %15s", method, uri, version);

        // Validate the method and URI
        if (!is_valid_method(method) || !is_valid_uri(uri)) {
            http_error(method, client_fd);
            close(client_fd);
            continue;
        }

        // Parse the body
        parse_body(buffer);

        // Handle the client's request based on the HTTP method
        handle_request(method, uri, client_fd);

        close(client_fd);
    }
    
    return 0;
}
