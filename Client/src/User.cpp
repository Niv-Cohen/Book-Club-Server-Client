//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//
#include <algorithm>
#include "../include/User.h"


User::User(string name): inventory(),takenBooks(),givenBooks(),genrebyId(),wishList(),name(name), connectionId(), subscriptionId(1), receiptId(1),connected(true),disconnectReceipt(),actionByReceipt(),_mutex(){

}

void User::addReceiptToMap(int receipt, string command) {
    _mutex.lock();
    actionByReceipt.insert(make_pair(receipt,command));
    _mutex.unlock();
}

void User::addBookToInventory(const string genre, const string bookName) {
    //_mutex.lock();
    //if the genre isnt in the map create a new vector
    bool added= false;
    //if the genre exists
    for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if (it->first == genre) {
            vector<BookInfo*> vec=it->second;
            vec.push_back(new BookInfo(bookName,this->getName(),genre));
            it->second=vec;
            added=true;
            break;
        }
    }
    //if the book wasnt added to the missing genre
    if(!added){
        vector<BookInfo*> vecInventory;
        vecInventory.push_back(new BookInfo(bookName,this->getName(),genre));
        inventory.insert(make_pair(genre,vecInventory));
        //create same genre list in all maps
        vector<BookInfo*> vecGiven;
        vector<BookInfo*> vecTaken;
        takenBooks.insert(make_pair(genre,vecTaken));
        givenBooks.insert(make_pair(genre,vecTaken));
    }
    //_mutex.unlock();
}

string User::getCommandFromMap(int receipt) {
    for (auto it = actionByReceipt.begin(); it != actionByReceipt.end(); it++) {
        if (it->first == receipt) {
            return it->second;
        }
    }
    return nullptr;
}

void User::moveToTakenBooks(BookInfo* book) {
    bool hasGenre=false;
    string genre=book->getGenre();
    for (auto it = takenBooks.begin(); it != takenBooks.end(); it++) {
           if(it->first == genre){
               vector<BookInfo*> vec=it->second;
               vec.push_back(book);
               it->second=vec;
               hasGenre=true;
               break;
        }
    }
    if(!hasGenre){
        vector<BookInfo*> vec;
        vector<BookInfo*> inventoryVec;
        vector<BookInfo*> givenVec;
        vec.push_back(book);
        takenBooks.insert(make_pair(genre,vec));
        inventory.insert(make_pair(genre,inventoryVec));
        givenBooks.insert(make_pair(genre,givenVec));
    }
}

/** search the book in inventory or in taken books*/
void User::moveToGivenBooks(string genre,string bookName) {
    _mutex.lock();
    bool found = false;
    bool done= false;
    //search by genre
    for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if (it->first == genre) {
        //    cout<<"found genre:"+it->first<<endl;
            vector<BookInfo*> vec=it->second;
            //search by book name in vector.
            for (auto it2 = vec.begin(); (it2 != vec.end()) & (!done); it2++) {
                //cout << "bookname requested:" + it2.operator*()->getName() << endl;
                if (it2.operator*()->getName() == bookName) {
                    found=true;
                    vector<BookInfo*> givenVec = givenBooks.at(genre);

                    givenVec.push_back(it2.operator*());
                    vec.erase(it2);
                    it->second = vec;
                    for(auto it4=givenBooks.begin(); it4!=givenBooks.end(); it4++){
                        if(it4.operator*().first==genre){
                            it4.operator*().second = givenVec;
                            done= true;
                            break;
                        }
                    }

                }
            }
        }
    }
    if(!found) {
        for (auto it3 = takenBooks.begin(); it3 != takenBooks.end(); it3++) {
            if (it3->first == genre) {
             //   cout << "found genre:" + it3->first << endl;
                vector<BookInfo *> vec = it3->second;
                for (auto it4 = vec.begin(); (it4 != vec.end()) & (!done); it4++) {
                    //cout << "bookname: " + it4.operator*()->getName() << endl;
                    if(it4.operator*()->getName() == bookName){
                        found = true;
                        for (auto it5 = givenBooks.begin(); it5 != givenBooks.end(); it5++){
                              if(it5->first==genre){
                                  vector<BookInfo*> takenVec=it5->second;
                                  takenVec.push_back(it4.operator*());
                                  it5->second=takenVec;
                                  vec.erase(it4);
                                  done=true;
                                  found=true;
                                  break;
                              }
                        }
                    }
                }
                it3->second = vec;
            }
        }
    }
    if(!found) {cout<<"the book was not found"<<endl;}
   _mutex.unlock();
}

string User::findLoaner(vector<string> input,string word) {
    int index=1;
    while(input[index]!=word){
        index++;
    }
    index++;
    return input[index];
}

bool User::onWishList(string bookName) {
    for (auto it = wishList.begin(); it != wishList.end(); it++){
       if(*it==bookName)
        return true;
    }
    return false;
}
void User::removeFromTakenBooks(BookInfo *book) {
    _mutex.lock();
    string genre = book->getGenre();
    string bookname = book->getName();
    for (auto it = takenBooks.begin(); it != takenBooks.end(); it++) {
        if(it->first == genre){
            vector<BookInfo*> vec=it->second;
            for(auto it2 = vec.begin(); it2!=vec.end(); it2++){
                if(it2.operator*()->getName() == bookname){
                    BookInfo *temp = *it2;
                    vec.erase(it2);
                    it->second=vec;
                    delete temp;
                    break;/** delete from heap*/
                }
            }

        }
    }
    _mutex.unlock();
}
/** maybe unscecry method*/
//void User::removeFromInvetoryToGivenBooks(string genre,string bookName) {
//    for (auto it = inventory.begin(); it != inventory.end(); it++){
//        if(it->first==genre){
//            vector<BookInfo*> vec=it->second;
//            for (auto it2 = vec.begin(); it2 != vec.end(); it2++){
//                string otherBookName=it2.operator*()->getName();
//                string lender=it2.operator*()->getOwner();
//                //found the right book
//                if(otherBookName==bookName){
//                        moveToGivenBooks(genre,bookName);
//                        vec.erase(it2);
//                        it->second = vec;
//                        break;
//                }
//            }
//        }
//    }
//}
void User::removeFromWishList(string bookName) {
    _mutex.lock();
    for (auto it = wishList.begin(); it != wishList.end(); it++){
        if(it.operator*()==bookName) {
            wishList.erase(it);
            break;
        }
    }
    _mutex.unlock();
}
/** serch the book in both inventory adn takenBooks maps*/
BookInfo* User::getBook(string genre, string bookName) {
    for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if (it->first == genre) {
         //   cout<<"found genre:"+it->first<<endl;
            vector<BookInfo*> vec=it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
            //    cout << "bookname requested:" + it2.operator*()->getName() << endl;
                if (it2.operator*()->getName() == bookName) {
                return *it2;
            }
            }
        }
    }
    for(auto it3 = takenBooks.begin(); it3!=takenBooks.end(); it3++) {
        if (it3->first == genre) {
        //    cout << "found genre:" + it3->first << endl;
            vector<BookInfo*> vec = it3->second;
            for (auto it4 = vec.begin(); it4 != vec.end(); it4++) {
         //       cout << "bookname requested:" + it4.operator*()->getName() << endl;
                if(it4.operator*()->getName() == bookName)
                    return it4.operator*();
            }
        }
    }
    cout<<"the book was not found"<<endl;
    return nullptr;
}
void User::addToWishList(string bookName) {
    wishList.push_back(bookName);
}
int User::getDisconnectReceipt() const { return disconnectReceipt;}

void User::setDisconnectReceipt(string receiptId) {
    disconnectReceipt=std::stoi(receiptId);
}
void User::setGenreToMap(string genre, int subId) {
    bool found= false;
    for (auto it = genrebyId.begin(); it != genrebyId.end(); it++) {
        if (it->first == genre) {
            found = true;
        }
    }
       if(!found){
           genrebyId.insert(make_pair(genre,subId));
        }
    }

string User::getName() const {
    return name;
}
int User::getSubscriptionIdAndIncrease() {
    _mutex.lock();
    int temp = subscriptionId;
    subscriptionId=subscriptionId+1;
    _mutex.unlock();
    return temp;
}

int User::getGenreId(string genre) {
    for (auto it = genrebyId.begin(); it != genrebyId.end(); it++) {
        if(it->first==genre){
            return it->second;
        }
    }
    cout<<"no genre found"<<endl;
    return 0 ;
}

int User::getReceiptIdAndIncrease() {
    _mutex.lock();
    int temp=receiptId;
    receiptId=receiptId+1;
    _mutex.unlock();
    return temp;
}

void User::logout() {
    connected=false;
}

bool User::isConnected() {
    return connected;
}
//
//void User::login() {
//    connected=true;
//}


string User::getStatus(string genre) {
    string status="";
    for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if(it->first==genre) {
            vector<BookInfo *> vec=it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
                   status=status+""+it2.operator*()->getName()+",";
            }
            break;
        }
        }
    for (auto it = takenBooks.begin(); it != takenBooks.end(); it++) {
        if(it->first==genre) {
            vector<BookInfo *> vec=it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
                status=status+""+it2.operator*()->getName()+",";
            }
            break;
        }
    }
    status=status.substr(0,status.size()-1);
    return status;
}

    bool User::removeFromMap(string genre) {
    _mutex.lock();
        for (auto it = genrebyId.begin(); it != genrebyId.end(); it++) {
            if (it->first == genre) {
                genrebyId.erase(it->first);
                _mutex.unlock();
                return true;
            }
        }
        _mutex.unlock();
        return false;
    }

    void User::moveFromGivenToLibarary(string genre, string bookName) {
        _mutex.lock();
        vector<BookInfo*> givenVec = givenBooks.at(genre);
        for(auto it=givenVec.begin(); it!=givenVec.end(); it++){
            if(it.operator*()->getName()==bookName){
                string owner = it.operator*()->getOwner();
                if(owner==this->getName()){//the book is originaly mine, put in inventory
                    addBookToInventory(genre,bookName);
                }
                else{
                    moveToTakenBooks(new BookInfo(bookName,owner,genre));
                }
                break;
            }
        }
        _mutex.unlock();
}
User::~User() {
    for(auto it=inventory.begin(); it!=inventory.end(); it++) {
        vector<BookInfo *> vec = it->second;
        for (auto bookInfo: vec) {
            delete (bookInfo);
        }
    }

    for(auto it=givenBooks.begin(); it!=givenBooks.end(); it++){
        vector<BookInfo*> givenVec=it->second;
            for (auto bookInfo: givenVec) {
                delete (bookInfo);
            }
        }

    for(auto it=takenBooks.begin(); it!=takenBooks.end(); it++){
        vector<BookInfo*> takenVec=it->second;
        for (auto bookInfo: takenVec) {
            delete (bookInfo);
        }
    }
}

