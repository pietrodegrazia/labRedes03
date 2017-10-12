#include <stdio.h>
#include <string.h>
#define SOCKET int
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv){
    struct sockaddr_in peer;
    SOCKET socket_fd;
    int port, peerlen, rc, i;
    char buffer[1250];
    
    if(argc < 2) {
        printf("Utilizar:\n");
        printf("rec -p <porta>\n");
        exit(1);
    }
    
    for(i=1; i < argc; i++) {
        if(argv[i][0] != '-'){
            printf("Parametro %d: %s invalido\n",i,argv[i]);
            exit(1);
        }
        switch(argv[i][1]){
            case 'p': // Numero IP
                i++;
                port = atoi(argv[i]);
                if(port < 1024) {
                    printf("Porta invalida\n");
                    exit(1);
                }
                break;
            default:
                printf("Parametro %d: %s invalido\n",i,argv[i]);
                exit(1);
        }
    }
    
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Falha na criacao do socket\n");
        exit(1);
    }
    
    memset((void *) &peer, 0, sizeof(struct sockaddr_in));
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = htonl(INADDR_ANY);
    peer.sin_port = htons(port);
    peerlen = sizeof(peer);
    
    if(bind(socket_fd,(struct sockaddr *) &peer, peerlen)) {
        exit(1);
        printf("Erro no bind\n");
    }
    
    printf("Socket inicializado. Aguardando mensagens...\n\n");
    
    while (1)
    {
        rc = recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *) &peer,(socklen_t *)&peerlen);
        printf("Recebido %s\n", &buffer);
        
        strcpy(buffer,"ACK");
        sendto(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&peer, peerlen);
        printf("Enviado ACK\n\n");
    }
}
