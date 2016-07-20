#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CLT_PORT    2951            // Слушающий порт клиентв
#define CLT_ADDR    "192.168.2.255" // Широковещательный адрес для рассылки
#define MSG_MAXLEN  64              // Максимальная длина передаваемого или
                                    // получаемого сообщения

int main(int argc, void *argv[])
{
    int                 sock;           // Сокет сервера    
    struct sockaddr_in  addr_clt;       // Данные слиентов
    char                msg[MSG_MAXLEN];// Отправляемое / получаемое сообщение
    socklen_t           addr_size;      // Размер  struct sockaddr_in
    int                 opt;            // Новое значение опции сокета

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
 
    /*
     * Заполняем данные для рассылки клиентам. При этом необходимо разрешить
     * посылку broadcast для сервера
     */
    addr_size = sizeof(addr_clt);
    addr_clt.sin_family = AF_INET;
    addr_clt.sin_port = htons(CLT_PORT);
    addr_clt.sin_addr.s_addr = inet_addr(CLT_ADDR);
    opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int)) == -1){
        perror("setsockopt");
        close(sock);
        exit(-1);   
    }

    while (1){
        printf("Send to clients: ");
        scanf("%s", msg);
        sendto(sock, msg, strlen(msg) + 1, 0, (struct sockaddr *)&addr_clt, addr_size);
        if (!strcmp(msg, "disconnect"))
            break;
    }

    close(sock);
    exit(0);
}
