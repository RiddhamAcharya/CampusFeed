#pragma once
#include <string>
#include <iostream>
using namespace std;
struct User{
    int id;
    string fullname;
    string email;
    string passwordHash;
    string role;
    bool isActive;
    int failedAttempts;
    long long lockedUntil;
    string createdAt;
    string updatedAt;
    string lastlogin;
};