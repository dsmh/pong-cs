CC=g++ -std=c++11
ZMQ=/home/dsm/zmq
SFML=/usr/local

ZMQ_INCS=$(ZMQ)/include
ZMQ_LIBS=$(ZMQ)/lib
SFML_INCS=$(SFML)/include
SFML_LIBS=$(SFML)/lib

all:  pong pong-server

pong: client.cc
	$(CC) -I$(SFML_INCS) -L$(SFML_LIBS) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o pong client.cc -lczmq -lzmq -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system

pong-server: server.cc
	$(CC) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o pong-server server.cc -lczmq -lzmq

clean:
	rm -f pong
	rm -f pong-server 	