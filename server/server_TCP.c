/*
 * Esonero di Reti di Calcolatori
 * TCP server
 * Author: Sonia Auciello
 */

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#define CLOSESOCKET closesocket
typedef int socklen_arg_t;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define CLOSESOCKET close
typedef socklen_t socklen_arg_t;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prot_appl.h"

#define NO_ERROR 0

static void clear_network(void) {
#if defined(_WIN32)
    WSACleanup();
#endif
}

static int parse_operation(const char *message,
                           char *operator,
                           double *operand1,
                           double *operand2) {
    char extra;
    int count = sscanf(message, " %c %lf %lf %c",
                       operator, operand1, operand2, &extra);
    return count == 3;
}

static int calculate(char operator,
                     double operand1,
                     double operand2,
                     double *result) {
    switch (operator) {
        case '+':
            *result = operand1 + operand2;
            return 1;
        case '-':
            *result = operand1 - operand2;
            return 1;
        case '*':
            *result = operand1 * operand2;
            return 1;
        case '/':
            if (operand2 == 0.0) {
                return 0;
            }
            *result = operand1 / operand2;
            return 1;
        default:
            return 0;
    }
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

#if defined(_WIN32)
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
        fprintf(stderr, "Errore durante WSAStartup().\n");
        return EXIT_FAILURE;
    }
#endif

    int port = PROTOPORT;
    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Porta non valida: %s\n", argv[1]);
            clear_network();
            return EXIT_FAILURE;
        }
    }

    int server_socket = (int) socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
        fprintf(stderr, "Creazione socket fallita.\n");
        clear_network();
        return EXIT_FAILURE;
    }

#ifndef _WIN32
    int reuse = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
               &reuse, sizeof(reuse));
#endif

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons((unsigned short) port);

    if (bind(server_socket, (struct sockaddr *) &server_addr,
             sizeof(server_addr)) < 0) {
        fprintf(stderr, "bind() fallita.\n");
        CLOSESOCKET(server_socket);
        clear_network();
        return EXIT_FAILURE;
    }

    if (listen(server_socket, 5) < 0) {
        fprintf(stderr, "listen() fallita.\n");
        CLOSESOCKET(server_socket);
        clear_network();
        return EXIT_FAILURE;
    }

    printf("Server TCP in ascolto su 127.0.0.1:%d\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_arg_t client_len = (socklen_arg_t) sizeof(client_addr);

        int client_socket = (int) accept(
            server_socket,
            (struct sockaddr *) &client_addr,
            &client_len
        );

        if (client_socket < 0) {
            fprintf(stderr, "accept() fallita.\n");
            continue;
        }

        printf("Client connesso: %s:%u\n",
               inet_ntoa(client_addr.sin_addr),
               (unsigned int) ntohs(client_addr.sin_port));

        char client_message[BUFF];
        char server_message[BUFF];

        while (1) {
            memset(client_message, 0, sizeof(client_message));
            int received = (int) recv(
                client_socket,
                client_message,
                sizeof(client_message) - 1,
                0
            );

            if (received <= 0) {
                printf("Connessione chiusa dal client.\n\n");
                break;
            }

            client_message[received] = '\0';
            printf("Ricevuto: %s\n", client_message);

            if (strcmp(client_message, "=") == 0) {
                printf("Connessione chiusa dal client.\n\n");
                break;
            }

            char operator;
            double operand1;
            double operand2;
            double result;

            if (!parse_operation(client_message,
                                 &operator,
                                 &operand1,
                                 &operand2)) {
                snprintf(server_message, sizeof(server_message),
                         "Formato non valido");
            } else if (operator == '/' && operand2 == 0.0) {
                snprintf(server_message, sizeof(server_message),
                         "Errore: divisione per zero");
            } else if (!calculate(operator,
                                  operand1,
                                  operand2,
                                  &result)) {
                snprintf(server_message, sizeof(server_message),
                         "Operatore non valido");
            } else {
                snprintf(server_message, sizeof(server_message),
                         "%.6g", result);
            }

            if (send(client_socket, server_message,
                     (int) strlen(server_message) + 1, 0) < 0) {
                fprintf(stderr, "Invio risposta fallito.\n");
                break;
            }
        }

        CLOSESOCKET(client_socket);
    }

    CLOSESOCKET(server_socket);
    clear_network();
    return EXIT_SUCCESS;
}
