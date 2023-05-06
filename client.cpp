#include"Socket.h"
#include"util.h"
#include<unistd.h>
#include<string.h>
#include"InetAddress.h"

#define READ_BUFFER 1024

int main(){
    Socket sock;
    InetAddress serv_addr("192.168.124.149", 9999);
    sock.connect(serv_addr);
    int sockfd = sock.getFd();
    while(true){
        char buf[READ_BUFFER];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, strlen(buf) + 1);
        if(write_bytes == -1){
            printf("server %d closed, can't write anymore!\n", sockfd);
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("receive message from server fd %d: %s\n", sockfd, buf);
        }else if(read_bytes == 0){
            printf("server fd %d close\n",sockfd); 
            break;
        }else if(read_bytes < 0){
            perror("read err\n");
            break;
        }
    }
    return 0;
}