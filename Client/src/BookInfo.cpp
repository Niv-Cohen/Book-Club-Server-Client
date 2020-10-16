//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include "../include/BookInfo.h"
//constructor
BookInfo::BookInfo(string bookName, string owner,string genre):bookName(bookName),genre(genre),lender(),owner(owner) {}
string BookInfo::getName() const {return this->bookName;}
//string BookInfo::getLender() const { return this->lender;}
//void BookInfo::updateLender(string name) {this->lender=name}
//bool BookInfo::bookStatus() {return hasBook;}
//void BookInfo::setBookStatus(bool status) {hasBook=status;}
string BookInfo::getOwner() {return owner;}
//void BookInfo::setOwenr(string ownerToSet) {ownerToSet=ownerToSet;}

string BookInfo::getGenre() const {
    return genre;
}
BookInfo::~BookInfo() {
}