#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET ((SOCKET)~0)
#define MAX_PACKET 1250

enum erros {WSTARTUP, ABRESOCK, BIND, ACCEPT, LISTEN,RECEIVE};

void TrataErro(SOCKET, int);

int main(int argc, char* argv[])
{
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int server_port, i;
    socklen_t addr_cli_len = sizeof(client);
    char recvbuf[MAX_PACKET];
    
    if(argc < 2) {
        printf("Utilizar:\n");
        printf("rec -p <porta do servidor>\n");
        exit(1);
    }
    
    for(i=1; i < argc; i++) {
        if(argv[i][0] != '-'){
            printf("Parametro %d: %s invalido\n",i,argv[i]);
            exit(1);
        }
        switch(argv[i][1]){
            case 'p': // porta
                i++;
                server_port = atoi(argv[i]);
                break;
            default:
                printf("Parametro %d: %s invalido\n",i,argv[i]);
                exit(1);
        }
    }
    
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        TrataErro(server_socket, ABRESOCK);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(server_port);
    
    if ((bind(server_socket, (struct sockaddr *)&server, sizeof(server))) != 0) {
        TrataErro(server_socket, BIND);
    }
    
    if((listen(server_socket, 8)) != 0) {
        TrataErro(server_socket, LISTEN);
    }
    
    if((client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&addr_cli_len)) < 0) {
        TrataErro(server_socket, ACCEPT);
    }
    
    while(1)
    {
        if ( ( recv(client_socket, recvbuf, MAX_PACKET, 0) ) < 0) {
            close(client_socket);
            TrataErro(server_socket, RECEIVE);
        }
        printf(" - msg recv - %s\n", recvbuf);
    }
    
    printf("Fim da conexao\n");
    close(server_socket);
    close(client_socket);
    exit(1);
}

void TrataErro(SOCKET s, int tipoerro)
{
    char tipo[20];
    
    switch(tipoerro) {
        case WSTARTUP:
            strcpy(tipo, "Windows Startup");
            break;
        case ABRESOCK:
            strcpy(tipo, "Open Socket");
            break;
        case BIND:
            strcpy(tipo, "Bind");
            break;
        case ACCEPT:
            strcpy(tipo, "Accept");
            break;
        case LISTEN:
            strcpy(tipo, "Listen");
            break;
        case RECEIVE:
            strcpy(tipo, "Receive");
            break;
        default:
            strcpy(tipo, "Indefinido. Verificar");
            break;
    }
    printf("erro no %s", tipo);
    close(s);
    exit(1);
}
