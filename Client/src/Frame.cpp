//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include "../include/Frame.h"
#include "../include/User.h"

Frame::Frame(string input):input(input) {}

  vector<string> Frame::split(string msg) {
    vector<string> split;
    while(msg.size()!=0){
        split.push_back(msg.substr(0,msg.find(" ")));
        if(msg.find(" ")!=string::npos) {
            msg = msg.substr(msg.find(" ") + 1);
        }
        else{
            msg="";
        }
    }
      return split;
}

vector<string> Frame::getSplit(string input) {
    return split(input);
}

string Frame::loginFrame(string name,string pass, string host) {
    string output="";
        output =output+ "CONNECT" + '\n';
        output =output+ "accept-version:1.2" + '\n';
        output =output+ "host:"+host+ '\n';
        output =output+ "login:" + name + '\n';
        output =output+ "passcode:"+pass + '\n';
        output =output+ '\n';
        output =output+ "\0" ;//null character
    return output;
    }

string Frame::createFrame(User* user) {
    vector<string> strs;
    strs=this->split(this->input);
    string command = input.substr(0, input.find(" "));
    string output="";
    string bookName="";
    int receipt=user->getReceiptIdAndIncrease();
    if (command == "join") {
        output =output + "SUBSCRIBE"+ '\n';
        output =output + "destination:" + strs[1] + '\n';
        int subId=user->getSubscriptionIdAndIncrease();
        user->setGenreToMap(strs[1],subId);
        output = output + "id:"+std::to_string(subId) + '\n';
        output =output+ "receipt:" +  std::to_string(receipt) + '\n'+'\n';
        output =output+ "\0";//null character
        user->addReceiptToMap(receipt,"Joined club "+strs[1] );
    }
    else if(command=="exit"){
        output =output + "UNSUBSCRIBE" + '\n';
        output = output + "id:" +  std::to_string(user->getGenreId(strs[1])) + '\n';
        output =output+ "receipt:" +  std::to_string(receipt) + '\n'+'\n';
        output =output+ "\0";//null character
        user->removeFromMap(strs[1]);
        user->addReceiptToMap(receipt,"Exited club "+strs[1] );
    }
    else if (command == "add") {
        output =output+ "SEND" + '\n';
        output =output+ "destination:" + strs[1] + '\n'+'\n';

        string bookName="";
        for (int i = 2; i <(int)strs.size() ; i++) {
          bookName=bookName+strs[i]+" ";
        }
        bookName=bookName.substr(0,bookName.size()-1);
        cout<<bookName<<endl;
        output =output+ user->getName() + " has added the book " + bookName+ '\n';
        output =output+ "\0";//null character
        user->addBookToInventory(strs[1],bookName);
    }
    else if (command == "borrow") {
        output =output+ "SEND" + '\n';
        output =output+ "destination:" + strs[1] + '\n'+'\n';
        string bookName="";
        for (int i = 2; i <(int)strs.size() ; i++) {
            bookName=bookName+strs[i]+" ";
        }
        bookName=bookName.substr(0,bookName.size()-1);
        user->addToWishList(bookName);
        output =output+ user->getName() + " wish to borrow " + bookName + '\n';
        output=output+'\n';
        output =output+ "\0";//null character
    }
        // TODO -> create a new frame in case there's some who give me this book
    else if (command == "return") {
        output = output+ "SEND" + '\n';
        output = output+ "destination:" + strs[1] + '\n'+'\n';
        for (int i = 2; i <(int)strs.size() ; i++) {
            bookName=bookName+strs[i]+" ";
        }
        bookName=bookName.substr(0,bookName.length()-1);
        BookInfo* bookToReturn = user->getBook(strs[1], bookName);
        string owner = bookToReturn->getOwner();
        output = output+ "Returning " + bookName + " to " + owner + '\n';
        output = output+ "\0";//null character
        /** Delete pointer of the book from taken books*/
        user->removeFromTakenBooks(bookToReturn);

    }
    else if (command == "status") {
        output = output+ "SEND" + '\n';
        output = output+ "destination:" + strs[1] + '\n'+'\n';
        output = output+ "book status"+'\n';
        output = output+ "\0";//null character
    }
    else if(command=="found"){
        output = output+ "SEND" + '\n';
        output = output+ "destination:" + strs[1] + '\n'+'\n';
        for (int i =2; i <(int) strs.size(); i++) {
            //cout << vec2[i] << endl;
            bookName = bookName + strs[i] + " ";
        }
        bookName=bookName.substr(0,bookName.length()-1);
        output = output+ user->getName()+" has the book "+bookName+ '\n';
        output = output+ "\0";//null character
    }
    else if(command=="Taking"){
        output = output+ "SEND" + '\n';
        output = output+ "destination:" + strs[1] + '\n'+'\n';
        int index=2;
        while(strs[index]!="from"){
            bookName=bookName+strs[index]+" ";
            index++;
        }
        bookName=bookName.substr(0,bookName.length()-1);
        output = output+"Taking "+bookName+" from "+strs[index+1]+ '\n';
        output = output+ "\0";//null character
        //user.getBook(strs[1],bookName)->updateLender(user.getName());
   //     cout<<"I AM "+user->getName()<<endl;
    }
    else if(command=="mystatus"){
        output=output+"SEND"+'\n';
        output = output+ "destination:" + strs[1] + '\n'+'\n';
        if((int)strs.size()>2) {
            output = output + user->getName() + ":";
            for (int i = 2; i < (int) strs.size(); i++) {
                output = output + strs[i] + " ";
            }
            output = output.substr(0, output.size() - 1);
        }
        else{
            output=output+user->getName()+":";
        }
        output=output+'\n';
        output = output+ "\0";
    }
    else if(command=="logout"){
        string disReceipt=std::to_string(receipt);
       user->setDisconnectReceipt(disReceipt);
       output=output+"DISCONNECT"+'\n';
       output=output+"receipt:"+disReceipt+'\n'+'\n';
       output = output+ "\0";
       user->logout();
    }
 //   cout<<output<<endl;
    return output;
}

