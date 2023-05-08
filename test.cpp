#include<iostream>
#include<unistd.h>
#include<string>
#include<functional>
#include"src/util.h"
#include"src/InetAddress.h"
#include"src/Socket.h"
#include"src/ThreadPool.h"
#include"src/Buffer.h"

using namespace std;
#define READ_BUFFER 1024

void oneClient(int msgs, int wait, int id){
    Socket sock;
    InetAddress addr("192.168.124.149", 9999);
    sock.connect(addr);
    int sockfd = sock.getFd();
    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    sleep(wait);
    int count = 0;
    while(count < msgs){
        char * ch = new char[20]{};
        sprintf(ch, "I'm client %d!", id);
        sendBuffer->setBuf(ch);
        delete ch;
        //sendBuffer->setBuf("I'm client :)!");
        
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
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

                readBuffer->append(buf, read_bytes);
                ready_read += read_bytes;

            }else if(read_bytes == 0){

                printf("server fd %d closed\n", sockfd); 
                exit(EXIT_SUCCESS);

            }else if(read_bytes < 0){
                perror("read err\n");
                break;
            }

            if(ready_read >= sendBuffer->size()){
                printf("count: %d, message from server: %s\n", count++, readBuffer->c_str());
                break;
            }
        }
        readBuffer->clear();
    }

    delete readBuffer;
    delete sendBuffer;
}

int main(int argc, char* argv[]){
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char * optstring = "t:m:w";
    while((o = getopt(argc, argv, optstring)) != -1){ //分析命令行参数 必须包含 optstring 提供的参数
        switch (o){
            case 't':
                threads = stoi(optarg);
                break;
            case 'm':
                msgs = stoi(optarg);
                break;
            case 'w':
                wait = stoi(optarg);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }

    ThreadPool *pool = new ThreadPool(threads);
    //function<void()> func = bind(oneClient, msgs, wait);
    for(int i = 0; i < threads; ++i){
        function<void()> func = bind(oneClient, msgs, wait, i);
        pool->add(func);
    }

    delete pool;
    return 0;
}