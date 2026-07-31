#include "password_service.h"
#include <argon2.h>
#include <random>
#include <stdexcept>
#include <iostream>
using namespace std;

vector<uint8_t>PasswordService::generateSalt(){
    vector<uint8_t>salt(SALT_LENGTH);
    std::random_device randomDevice;
    for(auto& byte:salt){
        byte=static_cast<uint8_t>(randomDevice());
    }
    return salt;
}

string PasswordService::hashPassword(const std::string& password){
    auto salt=generateSalt();
    char encoded[256];
    int result=argon2id_hash_encoded(
        TIME_COST,
        MEMORY_COST,
        PARALLELISM,
        password.c_str(),
        password.length(),
        salt.data(),
        salt.size(),
        HASH_LENGTH,
        encoded,
        sizeof(encoded)
    );
    if(result !=ARGON2_OK){
        throw runtime_error(
            argon2_error_message(result)

        );        
    }
    return string(encoded);
}

bool PasswordService::verifyPassword(
    const string& password,
    const string& storedHash
){
    return argon2id_verify(
        storedHash.c_str(),
        password.c_str(),
        password.length())==ARGON2_OK;
    )

}