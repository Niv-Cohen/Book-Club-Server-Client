


#include <iostream>
#include <thread>
#include <mutex>
#include "../include/StompBookClubClient.h"
#include "../include/User.h"
#include "../include/Frame.h"

//
// Created by nivri@wincs.cs.bgu.ac.il on 09/01/2020.
//
//login 132.72.45.157:6666 bob alice
//login 127.0.0.1:6666 bob alice
int main( int argc, const char* argv[] ) {


    std::cout << "enter command:" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    string name="";
    string pass="";
    short port;
    Frame frame = Frame(input);
    string command = input.substr(0, input.find(" "));
    if (command == "login") {
        input = input.substr(input.find(" ") + 1);
        string host = input.substr(0, input.find(":"));
        input = input.substr(input.find(":") + 1);
        vector<string> vec = frame.getSplit(input);
        if (vec.size() == 3) {
            port = std::stoi(vec[0]);
            name = vec[1];
            pass = vec[2];
        } else {
            cout << "wrong input" << endl;
            return 0;
        }

        User *user = new User(name);
        // std::mutex mutex1;
        ConnectionHandler *connectionHandler = new ConnectionHandler(host, port);
        if (connectionHandler->connect()) {
            string output = frame.loginFrame(name, pass,host);
            if (connectionHandler->sendLine(output)) {
                string response;
                if (connectionHandler->getLine(response)) {
                    cout << response << endl;
                    if (response.substr(0, response.find('\n')) != "ERROR") {
                        cout << "Login successful." << endl;
                        StompBookClubClient clientKeyBoard(user, connectionHandler);
                        StompBookClubClient clientSocket(user, connectionHandler);

                        std::thread KeyBoardThread(&StompBookClubClient::keyBoardInputThread, &clientKeyBoard);
                        std::thread SocketThread(&StompBookClubClient::socketThread, &clientSocket);
                        KeyBoardThread.join();
                        SocketThread.join();

                    }
                }

            }

        }
        else {
            cout << "Could not connect to server" << endl;
        }
        delete (connectionHandler);
        delete (user);
    }
}

