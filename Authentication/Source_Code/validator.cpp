#include <iostream>
#include "validator.h"
#include <regex>
#include <cctype>
using namespace std;

ValidationResult Validator::validateName(const string& name){
    if(name.empty()){
        return {false,"Full name is required."};
    }
    if(name.length()<2){
        return {false,"Name must contain at least 2 Characters."};

    }
    if (name.length()>100){
        return {false,"Name cannot exceed 100 characters."};
    }
    for(char ch:name){
        if(!std::isalpha(static_cast<unsigned char>(ch)))&&ch!=''){
            retrun{false,"Name can only contain letters and spaces."};
        }
    }
    return {true,"Valid"};
}

ValidationResult validator::validateEmail(const string& email){
    if(email.empty()){
        return {false,"Email is required."};
    }
    if(email.length()>254){
        return {false,"Email is too long."};
    }
    static const regex emailPattern(
         R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"
    );
    if(!std::regex_match(email,emailPattern)){
        return {false,"Invalid email format."};
    }
    return{true,"Valid"};
}
ValidationResult Validator::validatePassword(const std::string& password)
{
    if(password.length() < 8)
    {
        return {false, "Password must contain at least 8 characters."};
    }

    if(password.length() > 64)
    {
        return {false, "Password cannot exceed 64 characters."};
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for(char ch : password)
    {
        if(std::isupper(static_cast<unsigned char>(ch)))
            hasUpper = true;

        else if(std::islower(static_cast<unsigned char>(ch)))
            hasLower = true;

        else if(std::isdigit(static_cast<unsigned char>(ch)))
            hasDigit = true;

        else if(std::ispunct(static_cast<unsigned char>(ch)))
            hasSpecial = true;
    }

    if(!hasUpper)
    {
        return {false, "Password must contain at least one uppercase letter."};
    }

    if(!hasLower)
    {
        return {false, "Password must contain at least one lowercase letter."};
    }

    if(!hasDigit)
    {
        return {false, "Password must contain at least one number."};
    }

    if(!hasSpecial)
    {
        return {false, "Password must contain at least one special character."};
    }

    return {true, "Valid"};
}

