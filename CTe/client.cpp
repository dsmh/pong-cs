#include <czmq.h>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

void sendMsg(void* channel, vector<string> parts) {
  zmsg_t* msg = zmsg_new();
  for (const string& s : parts) {
    zmsg_addstr(msg, s.c_str());
  }
  zmsg_send(&msg, channel);
}

int main(int argc, char** argv) {

  if (argc != 2) {
    cerr << "Wrong execution!!" << endl << "./client ID" << endl;
    return 1;
  }

  zctx_t* context = zctx_new();
  void* client = zsocket_new(context, ZMQ_DEALER);
  zsocket_connect(client, "tcp://localhost:5555");

  // This is very strange, with this initialization the application wont work.
  //
  // zsock_t* client = zsock_new(ZMQ_DEALER);
  // int result = zsock_connect(client, "tcp://localhost:5555");
  // cout << "Connection result " << result << endl;

  string myName(argv[1]);
  zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};

  sendMsg(client, {"join", myName});

  // Wait for the game to start
  zmsg_t* msg = zmsg_recv(client);
  zmsg_print(msg);
  zmsg_destroy(&msg);

/***
 * Este ciclo basicamente verifica que algun mensaje sea recibido, y luego "juega"
 * */

  while (true) {
    int st = zmq_poll(items, 1, 10 * ZMQ_POLL_MSEC); //zmq_poll retorna -1 cuando se presiona ctrl+c
    if (st == -1) {
      // Handles termination by ^C
      break;
    }
    if (items[0].revents & ZMQ_POLLIN) { //Atiendo los mensajes recibidos
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      cout << "Incoming message:\n";
      zmsg_t* msg = zmsg_recv(client);  //Parte bloqueante, pero no se bloquea porque se ingresa con la condicion
      zmsg_print(msg);
      zmsg_destroy(&msg);
    }
    sendMsg(client, {"move", myName}); //Ejecucion del juego..... 
  }
  zctx_destroy(&context);///Falta destruir el socket
  return 0;
}
