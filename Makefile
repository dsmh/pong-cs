CC=g++
SFML=/usr
SFML_INCS=$(SFML)/include
SFML_LIBS=$(SFML)/lib
ZMQ=/home/utp/zmq
ZMQ_LIB=$(ZMQ)/lib
ZMQ_HDR=$(ZMQ)/include

all: pong pong-server

pong: client.cc
	$(CC) -I$(ZMQ_HDR) -L$(ZMQ_LIB) -I$(SFML_INCS) -L$(SFML_LIBS) -o pong client.cc  -lczmq -lzmq -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system

pong-server: server.cc
	$(CC) -I$(ZMQ_HDR) -L$(ZMQ_LIB) -o pong-server server.cc -lczmq -lzmq

clean:
	rm -f pong
	rm -f pong-server 	
