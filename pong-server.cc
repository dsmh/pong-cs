#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>



int main(void)
{
	//  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP); //LA CONSTANTE ES PARA RESPONDER SOLICITUDES
    int rc = zmq_bind (responder, "tcp://*:5555"); //*-> INTERFAZ DE RED POR DEFECTO
    assert (rc == 0);
    
    
    char buffer [100];
    printf(".............:::::::::::::::PONG ONLINE:::::::::::::::............. \n");
    zmq_recv (responder, buffer, 100, 0);
    zmq_send (responder, "done", 100, 0);
	return 0;
}
