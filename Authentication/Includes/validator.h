#pragma once 
#include <iostream>
#include <string>
using namespace std;
struct ValidationResult{
    bool sucess;
    std::string message;
};
class Validator{
    public:
    ValidationResult validateName(const string& name);
    ValidationResult validateEmail(const string& email);
    ValidationResult validatePassword(const string& password); 
};