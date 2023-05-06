#include"src/EventLoop.h"
#include"src/Server.h"

int main(){
    EventLoop loop;
    Server sever(loop, "192.168.124.149");
    //loop.loop();
    return 0;
}
        