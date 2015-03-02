CC=clang++ -std=c++11
ZMQ=/usr/local
SFML=/usr/local

ZMQ_INCS=$(ZMQ)/include
ZMQ_LIBS=$(ZMQ)/lib
SFML_INCS=$(SFML)/include
SFML_LIBS=$(SFML)/lib

all: pong server client

pong: pong.cpp
	$(CC) -I$(SFML_INCS) -L$(SFML_LIBS) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o pong pong.cpp -lczmq -lzmq -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system

server: server.cpp
	$(CC) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o server server.cpp -lczmq -lzmq

client: client.cpp
	$(CC) -I$(ZMQ_INCS) -L$(ZMQ_LIBS) -o client client.cpp -lczmq -lzmq

clean:
	rm -f pong server *.o *~	