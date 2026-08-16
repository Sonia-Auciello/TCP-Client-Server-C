/*
 * Esonero di Reti di Calcolatori
 * TCP client
 * Author: Sonia Auciello
 */

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#define CLOSESOCKET closesocket
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define CLOSESOCKET close
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

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

#if defined(_WIN32)
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
        fprintf(stderr, "Errore durante WSAStartup().\n");
        return EXIT_FAILURE;
    }
#endif

    const char *server_ip = "127.0.0.1";
    int port = PROTOPORT;

    if (argc >= 2) {
        server_ip = argv[1];
    }
    if (argc >= 3) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Porta non valida: %s\n", argv[2]);
            clear_network();
            return EXIT_FAILURE;
        }
    }

    int client_socket = (int) socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0) {
        fprintf(stderr, "Creazione socket fallita.\n");
        clear_network();
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short) port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1) {
        fprintf(stderr, "Indirizzo IP non valido: %s\n", server_ip);
        CLOSESOCKET(client_socket);
        clear_network();
        return EXIT_FAILURE;
    }

    if (connect(client_socket, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        fprintf(stderr, "Connessione al server fallita.\n");
        CLOSESOCKET(client_socket);
        clear_network();
        return EXIT_FAILURE;
    }

    printf("Connesso a %s:%d\n\n", server_ip, port);

    char client_message[BUFF];
    char server_message[BUFF];

    while (1) {
        printf(
            "Inserisci un'operazione nel formato: [operatore] [numero] [numero]\n"
            "Operatori validi: + - * /\n"
            "Per chiudere la connessione inserisci: =\n"
            "Input: "
        );

        if (fgets(client_message, sizeof(client_message), stdin) == NULL) {
            break;
        }

        client_message[strcspn(client_message, "\r\n")] = '\0';

        if (client_message[0] == '\0') {
            continue;
        }

        if (send(client_socket, client_message,
                 (int) strlen(client_message) + 1, 0) < 0) {
            fprintf(stderr, "Invio del messaggio fallito.\n");
            break;
        }

        if (strcmp(client_message, "=") == 0) {
            break;
        }

        memset(server_message, 0, sizeof(server_message));
        int received = (int) recv(client_socket, server_message,
                                  sizeof(server_message) - 1, 0);
        if (received <= 0) {
            fprintf(stderr, "Errore durante la ricezione dal server.\n");
            break;
        }

        server_message[received] = '\0';
        printf("Risultato: %s\n\n", server_message);
    }

    CLOSESOCKET(client_socket);
    clear_network();
    return EXIT_SUCCESS;
}
