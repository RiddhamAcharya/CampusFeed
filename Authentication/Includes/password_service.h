#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class PasswordService{
    public:
    string hashPassword(const string& password);
    bool verifyPassword(
        const string& password,
        const string& hash

    );
    private:
    vector<uint8_t>generateSalt();
    static constexpr uint32_t MEMORY_COST=65536;
    static constexpr uint32_t TIME_COST=3;
    static constexpr uint32_t PARALLELISM=2;
    static constexpr uint32_t HASH_LENGTH=32;
    static constexpr uint32_t SALT_LENGTH=16;
};
