//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_USER_H
#define ASSIGNMENT3CLIENT_USER_H

#include <unordered_map>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <mutex>
#include "../include/BookInfo.h"

using namespace std;
class User {

    public:
    User(string name);
    virtual ~User();
    void addReceiptToMap(int receipt,string command);
    void addBookToInventory(const string destination,const string bookName);
    string getName() const;
    int getSubscriptionIdAndIncrease();
    BookInfo* getBook(string genre,string bookName);
//    string getLenderName(string book) const;
    int getReceiptIdAndIncrease();
    void setGenreToMap(string genre,int subId);
    bool removeFromMap(string genre);
    int getGenreId(string genre);
//    bool hasBook(string bookName,string genre);
    void addToWishList(string bookName);
    bool onWishList(string bookName);
    void removeFromWishList(string bookName);
    void removeFromInvetoryToGivenBooks(string genre,string bookName);
    string findLoaner(vector<string> input,string word);
    void moveToTakenBooks(BookInfo *book);
    void moveToGivenBooks(string genre,string bookName);
//    void borrowBook(BookInfo* book);
//    void findBookAndMoveToGivenBooks(string genre,string bookName);
    void removeFromTakenBooks(BookInfo* book);
    string getStatus(string genre);
    void moveFromGivenToLibarary(string genre,string bookName);
//    void disconnect();
    void setDisconnectReceipt(string receiptId);
    int getDisconnectReceipt() const ;
    void logout();
    bool isConnected();
    void login();
    string getCommandFromMap(int receipt);


private:
    //<genre,books of that genre>
    unordered_map<string,vector<BookInfo*>> inventory;
    unordered_map<string,vector<BookInfo*>> takenBooks;//books i took from someone
    unordered_map<string,vector<BookInfo*>> givenBooks;//books i gave someone
    //<genre, subscription_id>
    unordered_map<string,int> genrebyId;
    //<books name, owners name>
    vector<string> wishList;
    string name;
    int connectionId;
    int subscriptionId;
    int receiptId;
    bool connected;
    int disconnectReceipt;
    unordered_map<int,string> actionByReceipt;
    mutex _mutex;
};


#endif //ASSIGNMENT3CLIENT_USER_H
