server : server.cpp src/EventLoop.cpp src/Server.cpp src/Channel.cpp src/Epoll.cpp  src/Socket.cpp src/InetAddress.cpp src/util.cpp
	g++ server.cpp src/EventLoop.cpp src/Server.cpp src/Channel.cpp src/Epoll.cpp  src/Socket.cpp \
	src/InetAddress.cpp src/util.cpp  -o server
	
clean:
	rm server
