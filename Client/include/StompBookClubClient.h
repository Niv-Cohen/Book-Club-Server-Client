//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_STOMPBOOKCLUBCLIENT_H
#define ASSIGNMENT3CLIENT_STOMPBOOKCLUBCLIENT_H

#include <string>
#include <iostream>
#include "../include/connectionHandler.h"
#include "../include/User.h"
using namespace std;

class StompBookClubClient {
    private:
    User* user;
    ConnectionHandler* connectionHandler;
    bool terminate;
    mutex  _mutex;
    void commandSwitch(string command);
    vector<string> splitByRows(string frame);
    vector<string> splitByRowsPrint(string frame);
    vector<string> splitFrame(string frame);
    public:

    StompBookClubClient(User* user, ConnectionHandler* ch);
    StompBookClubClient &operator=(const StompBookClubClient &stompBookClubClient);
    StompBookClubClient(StompBookClubClient& stompBookClubClient);
    virtual ~StompBookClubClient();
    User* getUser();
    void keyBoardInputThread();
    void socketThread();
    void terminateThread();
    void printResponseBody(string &response);
};


#endif //ASSIGNMENT3CLIENT_STOMPBOOKCLUBCLIENT_H
