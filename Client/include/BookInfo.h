//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_BOOKINFO_H
#define ASSIGNMENT3CLIENT_BOOKINFO_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class BookInfo {
public:
  BookInfo(string bookName,string owner,string genre);

    string getName() const;
    //string getLender() const;
    //string updateLender(string name);
    //bool bookStatus();
    //void setBookStatus(bool status);
    string getOwner();
    //void setOwenr(string ownerToSet);
    string getGenre() const;
    virtual ~BookInfo();
private:
    string bookName;
    string genre;
    string lender;
    string owner;
};



#endif //ASSIGNMENT3CLIENT_BOOKINFO_H
