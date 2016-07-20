#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CLT_PORT    2951            // Слушающий порт сервера
#define CLT_ADDR    "192.168.2.255" // Адрес сервера
#define MSG_MAXLEN  64              // Максимальная длина передаваемого или
                                    // получаемого сообщения

int main(int argc, void *argv[])
{
    int                 sock;           // Сокет клиента    
    struct sockaddr_in  addr_clt;       // Данные клиента
    struct sockaddr_in  addr_srv;       // Данные сервера
    char                msg[MSG_MAXLEN];// Получаемое сообщение
    socklen_t           addr_size;      // Размер  struct sockaddr_in

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
    
    /*
     * Поскольку клиент ждет широковещательную рассылку, то ему необходим
     * фиксорованный порт и соответсвующий адрес
     */
    addr_size = sizeof(addr_clt);
    addr_clt.sin_family = AF_INET;
    addr_clt.sin_port = htons(CLT_PORT);
    addr_clt.sin_addr.s_addr = inet_addr(CLT_ADDR);
    if (bind(sock, (struct sockaddr *)&addr_clt, addr_size) == -1){
        close(sock);
        perror("bind");
        exit(-1);
    }
    
    // Ждем broadcast сообщения от сервера
    while (1){
        if (recvfrom(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_srv, &addr_size) == -1){
            perror("recvfrom");            
            break;
        }
        if (!strcmp(msg, "disconnect"))
            break;
        printf("Received from server: %s\n", msg);
    }

    close(sock);
    exit(0);
}
