/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

void signal_handler(int alarm);

#include "Server.h"
#include<signal.h>
#include<unistd.h>

Server::Server(int port) throw (const char*) {
    stopClients = false;
    fd = socket(AF_INET, SOCK_STREAM, 0); // open socket
    if(fd < 0) // if socket() failed
        throw "socket failure";

    // define the server struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port); // to support both BIG,LITTLE endian systems

    if(bind(fd, (struct sockaddr*) &server, sizeof (server)) < 0) // bind the fd and the server
        throw "bind failure";

    if(listen(fd, 1) < 0) // ask listening to the server
        throw "listen failure";
}

void Server::start(ClientHandler& ch) throw (const char*) {
        // use lambda expression to start the thread
        t = new thread([&ch, this]() {
           signal(SIGALRM, signal_handler);
            // accept one client at a time until you stop the process
            while (!stopClients) {
                socklen_t clientSize = sizeof(client); // send client size to accept() by pointer for he can be changed
                alarm(1);
                int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
                if (aClient < 0) // if accept failed
                    throw "accept failure";

                ch.handle(aClient); // start process with the ClientHandler
                close(aClient);

                alarm(0); // cancel other alarms
            }
            close(fd); // we have finished with the clients
        });
}

// catch the SIGALRM
void signal_handler(int alarm) {
    return;
}

void Server::stop(){
    stopClients = true; // stop clients
	t->join(); // do not delete this!
}

Server::~Server() {}

