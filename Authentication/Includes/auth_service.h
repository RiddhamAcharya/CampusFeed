#pragma once 
#include <iostream>
using namespace std;
#include"user_repository.h"
#include"../models/user.h"

class AuthService{
    private:
    UserRepository& userRepository;
    public:
    AuthService(UserRepository& repo)
    :userRepository(repo){}
    bool registerUser(
         const string& fullName,
         const string& email,
         const string& password
        
    );


};

class AuthService{
    private:
    UserRepository& repository;
    PasswordService& passwordService;
    Validation& validator;
};
