#include"src/Socket.h"
#include"src/util.h"
#include<unistd.h>
#include<string.h>
#include"src/InetAddress.h"
#include"src/Buffer.h"

#define READ_BUFFER 1024
using namespace std;

int main(){
    Socket sock;
    InetAddress serv_addr("192.168.124.149", 9999);
    sock.connect(serv_addr);
    int sockfd = sock.getFd();

    Buffer sendBuffer, readBuffer;

    while(true){
        sendBuffer.getLine();
        ssize_t write_bytes = write(sockfd, sendBuffer.c_str(), sendBuffer.size());
        if(write_bytes == -1){
            printf("server %d closed, can't write anymore!\n", sockfd);
            break;
        }
        int ready_read = 0;
        char buf[READ_BUFFER];
        
        while(true){
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if(read_bytes > 0){
                
                readBuffer.append(buf, read_bytes);
                ready_read += read_bytes;
            }else if(read_bytes == 0){
                printf("server fd %d closed\n",sockfd); 
                exit(EXIT_SUCCESS);
            }else if(read_bytes < 0){
                perror("read err\n");
                break;
            }

            if(ready_read >= sendBuffer.size()){
                printf("receive message from server: %s\n", readBuffer.c_str());
                break;
            }
        }      
    }
    return 0;
}