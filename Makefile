CC=g++
SFML=/usr
SFML_INCS=$(SFML)/include
SFML_LIBS=$(SFML)/lib
ZMQ=/home/utp/zmq
ZMQ_LIB=$(ZMQ)/lib
ZMQ_HDR=$(ZMQ)/include

all: pong

pong: pong.cpp
	$(CC) -I$(ZMQ_HDR) -L$(ZMQ_LIB) -I$(SFML_INCS) -L$(SFML_LIBS) -o pong pong.cpp -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-system -lzmq

clean:
	rm -f pong 	
