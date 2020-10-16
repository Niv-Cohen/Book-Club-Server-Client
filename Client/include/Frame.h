//
// Created by nivri@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_FRAME_H
#define ASSIGNMENT3CLIENT_FRAME_H
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "User.h"

using namespace std;

class Frame {

   public:
    Frame(string input);
    string createFrame(User* user);
    vector<string> getSplit(string input);
    string loginFrame(string name,string pass,string host);
   private:
    string input;
   vector<string> split(string msg);
};


#endif //ASSIGNMENT3CLIENT_FRAME_H
