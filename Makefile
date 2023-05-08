src=$(wildcard src/*.cpp)
server : server.cpp $(src)
	g++ server.cpp $(src)  -o server -pthread -g
	
clean:
	rm server

client: client.cpp src/Buffer.cpp src/Socket.cpp src/util.cpp src/InetAddress.cpp
	g++ client.cpp src/Buffer.cpp src/Socket.cpp src/util.cpp src/InetAddress.cpp  -o client

threadTest:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test: test.cpp src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp
	g++ test.cpp src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp \
	-pthread  -o test  -g