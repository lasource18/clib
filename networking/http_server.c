#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 2048
#define PORT 8080

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int address_len = sizeof(address);
    char buffer[MAX_BUFFER_SIZE];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr*)&address, address_len) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    

        // Accept a client connection
        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&address_len)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read the client's response
        read(client_fd, buffer, MAX_BUFFER_SIZE);
        printf("%s\n", buffer);

        // Parse the HTTP method, the request URI, and the HTTP version
        char method[16], uri[256], version[16];
        sscanf(buffer, "%s %s %s", method, uri, version);

        // Find the Content-Length header
        char* content_length_ptr = strstr(buffer, "Content-Length: ");
        if (content_length_ptr != NULL)
        {
            int content_length = atoi(content_length_ptr + 16); // Skip past "Content-Length: "

            // Find the start of the body
            char* body_ptr = strstr(buffer, "\r\n\r\n");
            if (body_ptr != NULL)
            {
                body_ptr += 4; // Skip past "\r\n\r\n"

                // Read the rest of the body
                if (content_length > 0)
                {
                    char body[MAX_BUFFER_SIZE] = {0};
                    strncpy(body, body_ptr, strlen(body_ptr));

                    // Now body contains the body of the request
                    printf("Body: %s\n", body);     
                }  
            }
        }

        // Handle the client's request based on the HTTP method
        if (strcmp(method, "GET") == 0)
        {
            // Handle GET request
            if (strcmp(uri, "/") == 0)
            {
                char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, GET!";
                send(client_fd, response, strlen(response), 0);
            } else
            {
                char* response = "HTTP/1.1 404 Not Found\n";
                send(client_fd, response, strlen(response), 0);
            }
        } else if (strcmp(method, "POST") == 0)
        {
            // Handle POST request
            if (strcmp(uri, "/") == 0)
            {
                char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, POST!";
                send(client_fd, response, strlen(response), 0);
            } else
            {
                char* response = "HTTP/1.1 404 Not Found\n";
                send(client_fd, response, strlen(response), 0);
            }
        } else if (strcmp(method, "PUT") == 0)
        {
            // Handle PUT request
            if (strcmp(uri, "/") == 0)
            {
                char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, PUT!";
                send(client_fd, response, strlen(response), 0);
            } else
            {
                char* response = "HTTP/1.1 404 Not Found\n";
                send(client_fd, response, strlen(response), 0);
            }
        } else if (strcmp(method, "DELETE") == 0)
        {
            // Handle DELETE request
            if (strcmp(uri, "/") == 0)
            {
                char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, DELETE!";
                send(client_fd, response, strlen(response), 0);
            } else
            {
                char* response = "HTTP/1.1 404 Not Found\n";
                send(client_fd, response, strlen(response), 0);
            }
        } else
        {
            printf("Unkown HTTP method: %s\n", method);
            char* response = "HTTP/1.1 400 Bad Request\n";
            send(client_fd, response, strlen(response), 0);
        }

        close(client_fd);
    }
    
    return 0;
}
