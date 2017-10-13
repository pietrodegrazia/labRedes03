#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCKET int

int main(int argc, char **argv){
    struct sockaddr_in peer;
    SOCKET socket_fd;
    int port, peerlen, rc, i, kbits_to_send;
    float adjusted_sleep;
    char ip[16], buffer[1250];
    int buffer_bit_size = 10000;
    
    if(argc < 6) {
        printf("Utilizar:\n");
        printf("trans -h <numero_ip> -p <porta> -r <num_bits>\n");
        exit(1);
    }
    
    for(i=1; i<argc; i++) {
        
        if(argv[i][0] != '-') {
            printf("Parametro %d: %s invalido\n",i, argv[i]);
            exit(1);
        }
        
        switch(argv[i][1]) {
            case 'h': // Numero IP
                i++;
                strcpy(ip, argv[i]);
                break;
            case 'p': // porta
                i++;
                port = atoi(argv[i]);
                if(port < 1024) {
                    printf("Valor da porta invalido\n");
                    exit(1);
                }
                break;
            case 'r': //numero de bytes pra enviar em kbit/s
                i++;
                kbits_to_send = atoi(argv[i]);
                int bits_to_send = kbits_to_send * 1000;
                adjusted_sleep = (float)((float)buffer_bit_size / (float)bits_to_send) * 1000000;
                printf("Taxa: %f\n", adjusted_sleep);
                break;
            default:
                printf("Parametro invalido %d: %s\n",i,argv[i]);
                exit(1);
        }
    }
    
    if((socket_fd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
        printf("Falha na criacao do socket\n");
        exit(1);
    }
    
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    peer.sin_addr.s_addr = inet_addr(ip);
    peerlen = sizeof(peer);
    
    while(1)
    {
        strcpy(buffer,"Hello");
        sendto(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&peer, peerlen);
        printf("Enviado Hello\n");
        usleep(adjusted_sleep);
    }
}
