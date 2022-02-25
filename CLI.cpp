

#include "CLI.h"
#include <vector>
#include <string>
using namespace std;

CLI::CLI(DefaultIO* dio):dio(dio) {
    this->createCommandsVector();
}

void CLI::createCommandsVector() {
    this->commandsVector.push_back(new UploadCSV(this->dio));
    this->commandsVector.push_back(new AlgorithmSettings(this->dio));
    this->commandsVector.push_back(new DetectAnomalies(this->dio));
    this->commandsVector.push_back(new DisplayResults(this->dio));
    this->commandsVector.push_back(new UploadAnomaliesAnalyzeResult(this->dio));
    this->commandsVector.push_back(new Exit(this->dio));
}
void CLI::start() {
    int len = this->commandsVector.size();
    Info info;
    string choice = "0";
    while (choice != "6") {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < len; ++i) {
            dio->write(to_string(i+1) + ".");
            dio->write( this->commandsVector[i]->getDescription() + "\n");
        }

        // enter command number from the client.
        choice = dio->read();
        int c = stoi(choice);
        if(c-1 >=0 && c-1 <=6) {
            this->commandsVector[c - 1]->execute(&info);
        }

    }
}

CLI::~CLI() {
    for(size_t i=0; i<commandsVector.size(); i++) {
        delete commandsVector[i];
    }
}
