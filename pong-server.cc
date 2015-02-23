#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
const float pi = 3.14159f;
/*
float initBall()
{
      // Reset the ball angle
  do {
    // Make sure the ball initial angle is not too much vertical
    ballAngle = (std::rand() % 360) * 2 * pi / 360;
  } while (std::abs(std::cos(ballAngle)) < 0.7f);
}*/

int main(void)
{
	//  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP); //LA CONSTANTE ES PARA RESPONDER SOLICITUDES
    int rc = zmq_bind (responder, "tcp://*:5555"); //*-> INTERFAZ DE RED POR DEFECTO
    assert (rc == 0);
    
    
    char buffer [100];
    printf(".............:::::::::::::::PONG ONLINE:::::::::::::::............. \n");
    int i=0;
    while(1){
        zmq_recv (responder, buffer, 100, 0);
        zmq_send (responder, "done", 100, 0);
        cout << buffer << " " << i << endl;
        i++;
    }
    
	return 0;
}
