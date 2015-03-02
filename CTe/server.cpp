#include <czmq.h>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class ServerState {
private:
  zframe_t* playerAId;
  zframe_t* playerBId;

public:
  ServerState(void)
      : playerAId(nullptr)
      , playerBId(nullptr) {}
  bool complete() const { return playerAId != nullptr && playerBId != nullptr; }
  void playerJoins(zframe_t* playerIdentity) {
    assert(!complete());
    if (!playerAId)
      playerAId = zframe_dup(playerIdentity);
    else
      playerBId = zframe_dup(playerIdentity);
  }

  zframe_t* opponent(zframe_t* player) {
    char* idPlayer = zframe_strhex(player);
    char* idA = zframe_strhex(playerAId);
    char* idB = zframe_strhex(playerBId);

    zframe_t* opp = nullptr;

    if (strcmp(idPlayer, idA) == 0)
      opp = playerBId;
    else
      opp = playerAId;

    free(idPlayer);
    free(idA);
    free(idB);
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
  ServerState* state = reinterpret_cast<ServerState*>(_state);
  zmsg_t* msg = zmsg_recv(server);
  // zmsg_print(msg);

  zframe_t* identity = zmsg_pop(msg);
  zframe_t* action = zmsg_pop(msg);

  if (zframe_streq(action, "join")) {
    if (!state->complete()) {
      state->playerJoins(identity);
      sendMsg(server, identity, {"join accepted"});
    } else {
      cout << "Game is full!!" << endl;
      sendMsg(server, identity, {"Game full!!!"});
    }
  } else if (zframe_streq(action, "move")) {
    if (state->complete()) {
      char* playerName = zmsg_popstr(msg);
      sendMsg(server, state->opponent(identity),
              {"opponentMove", "fooo", playerName});
      free(playerName);
    }
  }