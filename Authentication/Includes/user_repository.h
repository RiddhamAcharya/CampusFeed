#pragma once
#include <iostream>
#include "database.h"
#include "../models/user.h"
using namespace std;
class UserRepository{
    private:
    Database& database;
    public:
    UserRepository(Database& db);
    bool emailExists(const string& emial);
    bool createUser(const User& user);
    bool getUserByEmail(const string& email,User& user);

};
