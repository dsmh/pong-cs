#include <czmq.h>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class ServerState {
//private:


  
public:

  zframe_t* playerAId;
  zframe_t* playerBId;

  zframe_t* playerCId;
  zframe_t* playerDId;

  int players;

  ServerState(void)
      : playerAId(nullptr)
      , playerBId(nullptr)
      , playerCId(nullptr)
      , playerDId(nullptr) {}
  bool complete() const { return playerAId != nullptr && playerBId != nullptr && playerCId != nullptr && playerDId != nullptr; }
  
  void playerJoins(zframe_t* playerIdentity) {
    
    assert(!complete());
    if (!playerAId){
      playerAId = zframe_dup(playerIdentity);
      cout <<"pA"<< playerAId << endl;
    }
    else if (playerAId && !playerBId){
      playerBId = zframe_dup(playerIdentity);
		cout <<"pB" << playerBId << endl;
    }else if (playerAId && playerBId && !playerCId){
      playerCId = zframe_dup(playerIdentity);
      cout <<"pC"<< playerCId << endl;
    }else if (playerAId && playerBId && playerCId && !playerDId)
      playerDId = zframe_dup(playerIdentity);
      cout <<"pD" << playerDId << endl;
    }


  vector<zframe_t*> opponents(zframe_t* player , zframe_t* playerAId, zframe_t* playerBId ,zframe_t* playerCId ,zframe_t* playerDId) {
    char* idPlayer = zframe_strhex(player);
    char* idA = zframe_strhex(zframe_dup(playerAId));
    char* idB = zframe_strhex(zframe_dup(playerBId));
    char* idC = zframe_strhex(zframe_dup(playerCId));
    char* idD = zframe_strhex(zframe_dup(playerDId));
    vector<zframe_t*> opp;

    if (strcmp(idPlayer, idA) == 0){
      opp.push_back(playerBId);
      opp.push_back(playerCId);
      opp.push_back(playerDId);
    }
    else if( strcmp(idPlayer, idB) == 0) 
      {
        opp.push_back(playerAId);
        opp.push_back(playerCId);
        opp.push_back(playerDId);
      }
    else if( strcmp(idPlayer, idC) == 0) 
      {
        opp.push_back(playerAId);
        opp.push_back(playerBId);
        opp.push_back(playerDId);
      }
    else{
        opp.push_back(playerAId);
        opp.push_back(playerBId);
        opp.push_back(playerCId);
      }
    

    free(idPlayer);
    free(idA);
    free(idB);
    free(idC);
    free(idD);
    return opp;
  }
};

void sendMsg(zsock_t* channel, zframe_t* to, vector<string> parts) {
  zmsg_t* msg = zmsg_new();
  zframe_t* dto = zframe_dup(to);
  zmsg_append(msg, &dto);
  for (const string& s : parts) {
    zmsg_addstr(msg, s.c_str());
  }
  zmsg_send(&msg, channel);
}

int handler(zloop_t*, zsock_t* server, void* _state) {
  ServerState *state = reinterpret_cast<ServerState*>(_state);
  zmsg_t* msg = zmsg_recv(server);
  zmsg_print(msg);

  zframe_t* identity = zmsg_pop(msg);
  zframe_t* active = zframe_dup(identity);
  zframe_t* action = zmsg_pop(msg);

  if (zframe_streq(action, "join")) {
    if (!state->complete()) {
      state->playerJoins(identity);

      if(state->complete()){

          cout << "ACTIVE IDS"<< endl; 

          /*cout << state->playerAId << endl;
          cout << state->playerBId << endl;
          cout << state->playerCId << endl;
          cout << state->playerDId << endl;*/
          
          ///////////////// BROADCAST SEND PLAYER NUMBER ON ARRIVAL///////////////////////

          sendMsg(server, state->playerAId, {"jugador1"});
          sendMsg(server, state->playerBId, {"jugador2"});
          sendMsg(server, state->playerCId, {"jugador3"});
          sendMsg(server, state->playerDId, {"jugador4"});
          
          //sendMsg(server, state->opponents(identity), {"jugador1"});

        }
    } else {
      cout << "Game is full!!" << endl;
      sendMsg(server, identity, {"Game full!!!"});
    }
  } else if (zframe_streq(action, "move")) {
    if (state->complete()) {


      
      //cout << "THE MUTHERFUCKER MESSAGED ITS BROKEN HERE"<<endl;
      char* new_pos = zmsg_popstr(msg);
      
      char* x = zmsg_popstr(msg);///new_pos es en realidad player newpos
      char* y = zmsg_popstr(msg);
      
      
      /*cout << "X:	"<< x <<endl;
      cout << "y:	"<< y <<endl;*/
      //zmsg_print(msg);
      ////BUSQUEDA DE enemigos
      vector<zframe_t*> enemigos = state->opponents(active, state->playerAId, state->playerBId, state->playerCId, state->playerDId);
          
          /*cout << "QUIEN:  " << active <<endl;
          cout <<"ENEMIGOS"<<endl<< enemigos[0] <<endl;
          cout << enemigos[1] <<endl;
          cout << enemigos[2] <<endl;*/

          sendMsg(server, enemigos[0],{"opponentMove", new_pos,x,y});
          sendMsg(server, enemigos[1],{"opponentMove", new_pos,x,y});
          sendMsg(server, enemigos[2],{"opponentMove", new_pos,x,y});

      //////ENVIAR MENSAJE A LISTA DE OPONENTES

      //sendMsg(server, state->opponent(identity),{"opponentMove", playerName}); //Eliminado "foo"
    
  
      
      free(new_pos);
    }
  } else if (zframe_streq(action, "ballpos"))
  {
    if (state->complete()) { 

      ///ARREGLAR LO DE POSICION DE LA BOLA ENVIADO EN BROADCAST Y RECIBIRLA ASINCRONAMENTE. DENTRO DE EL POLLIN.
	  //cout << "ZPRINT" << endl;
	  zmsg_print(msg);
	  
	  char* x = zmsg_popstr(msg);///new_pos es en realidad player newpos
      char* y = zmsg_popstr(msg);
	  
      vector<zframe_t*> enemigos = state->opponents(active, state->playerAId, state->playerBId, state->playerCId, state->playerDId);
          
          //cout << "QUIEN:  " << active <<endl;
          //cout <<"ENEMIGOS"<<endl<< enemigos[0] <<endl;
          //cout << enemigos[1] <<endl;
          //cout << enemigos[2] <<endl;

          sendMsg(server, enemigos[0],{"ballpos", x,y});
          sendMsg(server, enemigos[1],{"ballpos", x,y});
          sendMsg(server, enemigos[2],{"ballpos", x,y});
          

      
      }
  }else if (zframe_streq(action, "fin"))
  {
    if (state->complete()) { 

      vector<zframe_t*> enemigos = state->opponents(active, state->playerAId, state->playerBId, state->playerCId, state->playerDId);
      ///ARREGLAR LO DE POSICION DE LA BOLA ENVIADO EN BROADCAST Y RECIBIRLA ASINCRONAMENTE. DENTRO DE EL POLLIN.
    cout << "FIN" << endl;
    zmsg_print(msg);
    
    char* fin = zmsg_popstr(msg);///new_pos es en realidad player newpos
    
    sendMsg(server, enemigos[0],{"fin"});
    sendMsg(server, enemigos[1],{"fin"});  
    sendMsg(server, enemigos[2],{"fin"});
          

      
      }
  }
}

int main() {
  zsock_t *server = zsock_new(ZMQ_ROUTER);
  zsock_bind(server, "tcp://*:5555");
  
  ServerState *state = new ServerState();
  
  zloop_t *loop = zloop_new();
  zloop_reader(loop,server,&handler,state);
  zloop_start(loop);
  
  zsock_destroy(&server);
  return 0;
}












