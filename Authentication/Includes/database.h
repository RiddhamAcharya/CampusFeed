#include <iostream>
using namespace std;
#pragma once //Once Per compilation-prevent duplicate definition errors
#include <sqlite3.h>
#include <string>
//RAII (Resource Acquisition Is Initialization)
class Database{ //Creating class Name Database
    private:
    sqlite3* db; //Private Member variable sqlite3 object which represent the actual open database connection
    public:
    Database();
    ~Database();
    bool Database::connect(const std::string& path); //open connection to database
    void Database::close();//close the database connection
    bool execute(const std::string&sql); //To run a sql command
    sqlite3* getConnection(); //Return a raw sql pointer

};