/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include "CLI.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sstream>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO:public DefaultIO{
    int clientID;

public:

    // constructor
    SocketIO(int client_id):clientID(client_id) {}

    virtual string read() {

        char buf = 0;
        string input = "";
        recv(clientID, &buf, sizeof (char), 0);
        while(buf != '\n') {
            input += buf;
            recv(clientID, &buf, sizeof (char), 0);
        }
        return input;
    }

    virtual void write(string text) {
        // write to client

        const char* text2 = text.c_str();
        send(clientID, text2, strlen(text2), 0);
    }

    virtual void write(float f) {
        // convert float to string and overload method

        ostringstream sstr;
        sstr <<f;
        string s(sstr.str());
        write(s);
    }

    virtual void read(float* f) {
        // read float
        recv(clientID, f, sizeof (float), 0);
    }

    virtual ~SocketIO() = default;
};



// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{

	public:
    virtual void handle(int clientID){
        SocketIO socketIo(clientID);
        CLI cli (&socketIo);
        cli.start();
    }
};

class Server {
	thread* t; // the thread to run the start() method in
    sockaddr_in server;
    sockaddr_in client;
    int fd; // file descriptor
    volatile bool stopClients; // client loop flag

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch) throw (const char*);
	void stop();
};

#endif /* SERVER_H_ */
