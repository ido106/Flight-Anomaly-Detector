/**
Liron Balastra 206963316
Ido Aharon 319024600
**/

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
using namespace std;

class CLI {
	DefaultIO* dio;
    vector<Command*> commandsVector;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
private:
    void createCommandsVector();
};

#endif /* CLI_H_ */
