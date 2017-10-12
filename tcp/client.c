#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET ((SOCKET)~0)

#define CLIENT_PORT 2345 // porta TCP do cliente

int main(int argc, char* argv[]) {
    SOCKET socket_fd;
    struct sockaddr_in client, server;
    int i, server_port;
    char server_ip[16], buffer[1250];
    
    if(argc < 3) {
        printf("Utilizar:\n");
        printf("rec -h <ip do servidor> -p <porta do servidor>\n");
        exit(1);
    }
    
    for(i=1; i < argc; i++) {
        if(argv[i][0] != '-'){
            printf("Parametro %d: %s invalido\n",i,argv[i]);
            exit(1);
        }
        switch(argv[i][1]){
            case 'h': // Numero IP
                i++;
                strcpy(server_ip, argv[i]);
                break;
            case 'p': // porta
                i++;
                server_port = atoi(argv[i]);
                break;
            default:
                printf("Parametro %d: %s invalido\n",i,argv[i]);
                exit(1);
        }
    }
    
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Erro iniciando socket\n");
        return(0);
    }
    
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(CLIENT_PORT);
    
    if ((bind(socket_fd, (struct sockaddr *)&client, sizeof(client))) != 0) {
        printf("erro no bind\n");
        close(socket_fd);
        return(0);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);
    
    if(connect(socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0) {
        printf("erro na conexao");
        close(socket_fd);
        exit(1);
    }
    
    strcpy(buffer, "Hello World!\0");
    while(1) {
        if ( ( send(socket_fd, buffer, sizeof(buffer), 0) ) < 0 ) {
            printf("erro na transmissao\n");
            close(socket_fd);
            return 0;
        }
    }

    printf("Fim da conexao\n");
    close(socket_fd);
    return 0;
}
