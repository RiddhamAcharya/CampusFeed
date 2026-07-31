#pragma once
#include <iostream>
#include "user_repository.h"
#include "password_service.h"
#include "validator.h"
using namespace std;


class AuthService{
    private:
    UserRepository& userRepository;
    PasswordService& passwordService;
    Validator& validator;
    public:
    AuthService(
        UserRepository& repo,
        PasswordService& password,
        Validator& Validator
    );
    ValidationResult registerUser(
        const string& fullName,
        const string& email,
        const string& password

    );
};
ValidationResult AuthService::registerUser(
    const std::string& fullName,
    const std::string& email,
    const std::string& password)
{
    auto nameResult = validator.validateName(fullName);

    if(!nameResult.success)
        return nameResult;

    auto emailResult = validator.validateEmail(email);

    if(!emailResult.success)
        return emailResult;

    auto passwordResult = validator.validatePassword(password);

    if(!passwordResult.success)
        return passwordResult;

    if(userRepository.emailExists(email))
    {
        return {false, "Email already registered."};
    }

    User user;

    user.fullName = fullName;

    user.email = email;

    user.passwordHash =
        passwordService.hashPassword(password);

    user.role = "student";

    if(!userRepository.createUser(user))
    {
        return {false, "Unable to create account."};
    }

    return {true, "Registration successful."};
}

