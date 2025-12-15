#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "myproto.h"

#define MAX_EVENTS 64
#define MAX_BUFFER_SIZE 256 * 1024
#define PORT 8080

int main(int argc, char** argv) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Failed while setting socket up");
        return 1;
    }

    int reuse_port = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_port, sizeof(reuse_port));

    int binding = bind(socket_fd, (const struct sockaddr*)&server, sizeof(server));
    if (binding == -1) {
        perror("Failed while binding IPV4 socket");
        return 1;
    }

    int listening = listen(socket_fd, 1);
    if (listening == -1) {
        perror("Failed to listen port");
        return 1;
    }

    printf("Using MyProto version " MP_VERSION "\n");
    printf("Listening on port %d\n", PORT);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Failed while creating epoll file descriptor");
        return 1;
    }

    struct epoll_event epoll_ev;
    struct epoll_event events[MAX_EVENTS];

    epoll_ev.events = EPOLLIN | EPOLLET;
    epoll_ev.data.fd = socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &epoll_ev) == -1) {
        perror("Could not add file descriptor to epoll");
        return 1;
    }

    myproto_client* clients[100];

    while (1) {
        int ready_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < ready_events; i++) {
            int main_fd = events[i].data.fd;

            // printf("main_fd = %d;\n", main_fd);

            if (main_fd == socket_fd) {
                struct sockaddr_in client;
                socklen_t length = sizeof(client);

                int client_fd = accept(socket_fd, (struct sockaddr*)&client, &length);

                if (client_fd == -1) {
                    perror("Could not connect to client");
                    continue;
                }

                int current_flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, current_flags | O_NONBLOCK);

                char welcome_message[64];
                sprintf(welcome_message, "Hello! You are the file descriptor %d", client_fd);

                send(client_fd, welcome_message, strlen(welcome_message), MSG_NOSIGNAL);

                struct epoll_event epoll_ev_client;

                epoll_ev_client.events = EPOLLIN | EPOLLET;
                epoll_ev_client.data.fd = client_fd;

                clients[client_fd - 3] = malloc(sizeof(myproto_client));
                clients[client_fd - 3]->buffer_size = MAX_BUFFER_SIZE;
                clients[client_fd - 3]->data_buffer = malloc(sizeof(uint8_t) * MAX_BUFFER_SIZE);
                clients[client_fd - 3]->data_length = 0;

                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &epoll_ev_client);
            } else {
                myproto_client* client = clients[main_fd - 3];
                if (client == NULL) continue;
                char buffer[MAX_BUFFER_SIZE];
                int received = recv(main_fd, buffer, MAX_BUFFER_SIZE, 0);

                if (received <= 0)
                    goto close;

                printf("Received bytes: %d\n\n", received);

                if (received + client->data_length > client->buffer_size) {
                    client->buffer_size = client->data_length + received;
                    client->data_buffer = realloc(client->data_buffer, sizeof(uint8_t) * client->buffer_size);
                    printf("Client %d's buffer size increased to %ld bytes\n", main_fd, client->buffer_size);
                }

                memcpy(client->data_buffer + client->data_length, buffer, MAX_BUFFER_SIZE);
                client->data_length += received;
                // printf("buffer size = %ld\n", client->buffer_size);
                printf("data length = %ld\n", client->data_length);
                // for (size_t i = 0; i < client->data_length; i++)
                //     printf("buf[%ld] = %d (byte %ld)\n", i, client->data_buffer[i], i + 1);
                // printf("Received.\n");
                myproto_data data;
                int result = myproto_parse_bytes(&data, client->data_buffer, client->data_length);
                if (result == -1) {
                    printf("Error while parsing bytes. Closing connection with %d\n", main_fd);
                    goto close;
                }
                char data_return[32];
                sprintf(data_return, "\nServer: " MP_VERSION "\nClient: %d.%d.%d", data.version.major, data.version.minor, data.version.patch);
                send(main_fd, data_return, strlen(data_return), 0);
                for (size_t i = 0; i < 2; i++) {
                    printf("field %s = %s;\n", data.fields[i].key, data.fields[i].value);
                }
                switch (data.method) {
                    case MP_MET_CLOSE:
                        goto close;
                        break;
                }
                goto debuff;
                continue;
            debuff:
                printf("Reseting buffer!\n");
                client->data_length = 0;
                client->buffer_size = MAX_BUFFER_SIZE;
                client->data_buffer = realloc(client->data_buffer, sizeof(uint8_t) * MAX_BUFFER_SIZE);
                continue;
            close:
                printf("Bye client! %d\nClosed with %d\nerrno = %d;\n", main_fd, received, errno);
                free(client->data_buffer);
                free(client);
                clients[main_fd - 3] = NULL;
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, main_fd, NULL);
                shutdown(main_fd, SHUT_RDWR);
                close(main_fd);
            }
        }
    }

    close(socket_fd);

    return 0;
}
