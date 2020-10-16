//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//
#include <iostream>
#include <mutex>
#include <thread>
#include "../include/StompBookClubClient.h"
#include "../include/connectionHandler.h"
#include "../include/Frame.h"
User* StompBookClubClient::getUser() {
    return user;}

StompBookClubClient::~StompBookClubClient() {

}
StompBookClubClient::StompBookClubClient(User* user,ConnectionHandler* ch):user(user),connectionHandler(ch),terminate(false),_mutex(){}
StompBookClubClient::StompBookClubClient(StompBookClubClient &stompBookClubClient):user(stompBookClubClient.user),connectionHandler(stompBookClubClient.connectionHandler),terminate(stompBookClubClient.terminate),_mutex() {
}
StompBookClubClient& StompBookClubClient::operator=(const StompBookClubClient &stompBookClubClient) {
         if(this != &stompBookClubClient){
            user = stompBookClubClient.user;
            //connectionHandler=stompBookClubClient.connectionHandler;
            terminate=stompBookClubClient.terminate;
        }
        return *this;
    }


void StompBookClubClient::keyBoardInputThread() {
    while (!this->terminate) {
        while (this->user->isConnected()) {
            //std::lock_guard<std::mutex> lock(_mutex);
            std::string input;
      //      std::cout<<"enter command:"<<std::endl;
            std::getline(std::cin, input);
            Frame msg = Frame(input);
            string msgToServer = msg.createFrame(getUser());
            //std::cout<<"sending frame"<<std::endl;
            _mutex.lock();
        //    cout<<"sending"+msgToServer<<endl;
            connectionHandler->sendLine(msgToServer);
            _mutex.unlock();
        }
        terminateThread();
    }
    cout<<"keyBoard terminated"<<endl;
}

void StompBookClubClient::socketThread() {

    while(!this->terminate){
       // cout<<"waiting for message in socket thread"<<endl;
        //std::lock_guard<std::mutex> lock(_mutex);

        string response;
        _mutex.lock();
        if(this->connectionHandler->getLine(response)){
            _mutex.unlock();
     //       cout<<response<<endl;
            string header=response.substr(0,response.find('\n'));
            string printableResponse = response;
            if (header=="MESSAGE"){
                printResponseBody(printableResponse);
            }
            //string header=response.substr(0,'\n');
            //received an ERROR from server.terminate thread.
            if(header=="ERROR")
            {
           //     cout<<"ERROR was received"<<endl;
    //            std::cout<<response<<std::endl;
                this->user->logout();
                terminateThread();
            }
            else {
                commandSwitch(response);
            }
        }


    }
    cout<<"socket terminated"<<endl;

    }

 void StompBookClubClient::printResponseBody(string &response) {
    string header=response.substr(0,response.find('\n'));
    vector<string> split = splitByRowsPrint(response);
    if(split.size()>=6) {
        string topic = split[3].substr(split[3].find(":") + 1, split[3].size());
        std::cout << topic + ":" + split[5] << std::endl;
    }
}
vector<string> StompBookClubClient::splitByRowsPrint(string frame) {
    vector<string> split;
    while(frame.size()!=0){
        split.push_back(frame.substr(0,frame.find('\n')));
        if(split[split.size()-1]!=""){
            frame = frame.substr(frame.find('\n') + 1);
        }
        else{
            split.push_back(frame.substr(1,frame.length()-2));
            frame="";
        }
    }
    return split;
}
vector<string> StompBookClubClient::splitByRows(string frame) {
    vector<string> split;
    while(frame.size()!=0){
        split.push_back(frame.substr(0,frame.find('\n')));
        if(frame.find(" ")!=string::npos) {
            frame = frame.substr(frame.find('\n') + 1);
        }
        else{
            frame="";
        }
    }
    return split;
}
vector<string> StompBookClubClient::splitFrame(string frame) {
    vector<string> split;
    while(frame.size()!=0){
        split.push_back(frame.substr(0,frame.find('\n')));
            frame = frame.substr(frame.find('\n') + 1);
    }
    return split;

}
//login 132.72.45.169:1234 niv nivos
void StompBookClubClient::commandSwitch(string command) {
    string commandHeader=command.substr(0,command.find('\n'));
    if(commandHeader=="RECEIPT"){
        vector<string> vec=splitFrame(command);
            string id=vec[1].substr(vec[1].find(":")+1);
            if(std::stoi(id)==user->getDisconnectReceipt()){
                terminateThread();
            }
            else{
                int receipt=std::stoi(vec[1].substr(vec[1].find(":")+1));
                cout<<user->getCommandFromMap(receipt)<<endl;
            }
        }
    else if (commandHeader=="CONNECTED") {
        cout << "connetcion was susscesful" << endl;
    }
    else{
        vector<string> vec = splitByRows(command);
         if (vec[0] == "MESSAGE") {
            if (vec.size() == 1) {
                /** Nothing to do, recieved a book status, no action needed*/
            }
            else if (vec[5].find(" ") == string::npos) {
                cout << "no action is needed" << endl;
            }
            else {
                Frame tempFrame = Frame(vec[5]);
                vector<string> vec2 = tempFrame.getSplit(vec[5]);
                string bookName = "";
                if (vec2[1] == "status") {
                    string genre = vec[3].substr(vec[3].find(":") + 1);
                    string status = user->getStatus(genre);
                    string msg = "mystatus " + genre + " " + status;
                    Frame frame = Frame(msg);
                    string msgToServer = frame.createFrame(getUser());
                    _mutex.lock();
                    connectionHandler->sendLine(msgToServer);
                    _mutex.unlock();

                }
                else if (vec2[1] == "wish") {
                    for (int i = 4; i < (int)vec2.size(); i++) {
                  //      cout << vec2[i] << endl;
                        bookName = bookName + vec2[i] + " ";
                    }
                    bookName = bookName.substr(0, bookName.size() - 1);//cut the first space of the name
                    string genre = vec[3].substr(vec[3].find(":") + 1);
                //    cout << "bookname requested:" + bookName + ", genre:" + genre << endl;
                    BookInfo *bookInfo = this->user->getBook(genre, bookName);
                    if (bookInfo != nullptr) {
                        string msg = "found " + genre + " " + bookName;
                        Frame frame2 = Frame(msg);
                        string msgToServer = frame2.createFrame(getUser());
                        _mutex.lock();
                        connectionHandler->sendLine(msgToServer);
                        _mutex.unlock();
                    }
                } else if (vec2[0] == "Taking")//someone takes a book from me
                {
                    int index = 1;
                    string bookName = "";
                    while (vec2[index] != "from") {
                        bookName = bookName + vec2[index] + " ";
                        index++;
                    }
                    string genre = vec[3].substr(vec[3].find(":") + 1);
                    bookName = bookName.substr(0, bookName.size() - 1);
                    string loaner = user->findLoaner(vec2, "from");
              //      cout << "book name:" + bookName << endl;
                    //cout<<"loaner name:"+loaner<<endl;
                    if (user->getName() == loaner) {
                        cout << "found loner:" + loaner << endl;

                        /**find the book in inventory or in takenBooks and move to given books*/
                        user->moveToGivenBooks(genre, bookName);//TODO
                    //    cout << loaner + " removed " + bookName + " from library";
                    }
                } else if (vec2[2] == "added") {
                    string owner = vec2[0];
                    for (int i = 5; i <(int) vec2.size(); i++) {
                //        cout << vec2[i] << endl;
                        bookName = bookName + vec2[i] + " ";
                    }
                    string genre = vec[3].substr(vec[3].find(":") + 1);
                    bookName = bookName.substr(0, bookName.size() - 1);
                    if (user->onWishList(bookName)) {
                        string msg = "Taking " + genre + " " + bookName + " from " + owner;
                        //   user->addBookToInventory(genre,bookName);
                        _mutex.lock();
                        user->moveToTakenBooks(new BookInfo(bookName, owner, genre));
                        _mutex.unlock();
                        cout << "adding " + bookName + " to inventory";
                        user->removeFromWishList(bookName);
                        Frame frame2 = Frame(msg);
                        string msgToServer = frame2.createFrame(getUser());
                        _mutex.lock();
                        connectionHandler->sendLine(msgToServer);
                        _mutex.unlock();
                    }
                } else if (vec2[1] == "has") {/**check if client wanted this book */
                    string owner = vec2[0];
                    for (int i = 4; i <(int)vec2.size(); i++) {
               //         cout << vec2[i] << endl;
                        bookName = bookName + vec2[i] + " ";
                    }

                    string genre = vec[3].substr(vec[3].find(":") + 1);
                    bookName = bookName.substr(0, bookName.size() - 1);
                    if (user->onWishList(bookName)) {/** book was indeed in wishlist, adding to taken books*/
                        string msg = "Taking " + genre + " " + bookName + " from " + owner;
                        _mutex.lock();
                        user->moveToTakenBooks(new BookInfo(bookName, owner, genre));
                        _mutex.unlock();
                        //cout << "adding " + bookName + " to takenBooks"<<endl;
                        user->removeFromWishList(bookName);
                        Frame frame2 = Frame(msg);
                        string msgToServer = frame2.createFrame(getUser());
                        _mutex.lock();
                        connectionHandler->sendLine(msgToServer);
                        _mutex.unlock();
                    }
                }
                else if (vec2[0] == "Returning")
                {
                    if (vec2[vec2.size() - 1] == user->getName()) {/**someones returns me the book back*/
                        int index = 1;
                        string bookName = "";
                        while (vec2[index] != "to") {
                            bookName = bookName + vec2[index] + " ";
                            index++;
                        }
                        string genre = vec[3].substr(vec[3].find(":") + 1);
                        bookName = bookName.substr(0, bookName.size() - 1);
                        user->moveFromGivenToLibarary(genre, bookName);

                    }
                }
            }
        }
    }
}
void StompBookClubClient::terminateThread() {
    terminate=true;
}



