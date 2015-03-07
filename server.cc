#include <czmq.h>
#include <iostream>

using std::cout;
using std::endl;


zframe_t* findOpponent(zframe_t * idMove, zframe_t *idA, zframe_t *idB) {
  char* idM = zframe_strhex(idMove);
  char* id1 = zframe_strhex(idA);
  char* id2 = zframe_strhex(idB);

  if(strcmp(idM,id1) == 0)
    return zframe_dup(idB);
  return zframe_dup(idA);
}

zmsg_t * prepareStart(zframe_t* id) {
  zmsg_t *m = zmsg_new();
  zmsg_addstr(m, "start");
  zframe_t *a = zframe_dup(id);
  zmsg_prepend(m, &a);
  return m;
}

void broadcastStart(zsock_t *channel, zframe_t *idA, zframe_t *idB) {
  zmsg_t *ma = prepareStart(idA); 
  zmsg_send(&ma,channel);
  zmsg_t *mb = prepareStart(idB); 
  zmsg_send(&mb,channel);
}

int main (void)
{
    zsock_t *server = zsock_new_router("tcp://*:5555");
    int players = 0;
    zframe_t *playerAId;
    zframe_t *playerBId;
    
    while(true) {
        zmsg_t *msg = zmsg_recv(server);
        zmsg_print(msg);
        zframe_t *id = zmsg_pop(msg);
        char *op = zmsg_popstr(msg);
        if (strcmp(op, "registro") == 0) {
            switch(players){
                case 2:
                  cout << "Ocupados" << endl;
                  break;
                case 1: 
                  playerBId = zframe_dup(id);
                  players++;
                  broadcastStart(server,playerAId,playerBId);
                  cout << "Game starts!!" << endl;
                  break;
                case 0:
                  playerAId = zframe_dup(id);
                  players++;
                  break;
            }       
        } else {
          cout << "Movement!!!!" << endl;
          zframe_t *move = zmsg_pop(msg);
          zframe_t *dest = findOpponent(id,playerAId, playerBId);
          zmsg_t *resp = zmsg_new();
          zmsg_prepend(resp, &move);
          zmsg_prepend(resp, &dest);
          
          zmsg_send(&resp, server);
          
        }
        zframe_destroy(&id);
    }
    
    zframe_destroy(&playerAId);
    zframe_destroy(&playerBId);
    zsock_destroy (&server);
    return 0;
}
