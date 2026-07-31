#include"../Includes/database.h"
#include <iostream>
using namespace std;

Database::Database(){
    db=nullptr;
}
Database::~Database(){
    close();
}
bool Databse::connect(const std::string& path){
    int result=sqlite3_open(path.c_str(),&db);
    if(result !=SQLITE_OK){
        cerr<<"Cannot open database"<<endl;
        return false;
    }
    cout<<"Database connected"<<endl;
    return true;
}
bool Database::execute(const std::string& sql){
    char* erroMessage=nullptr;
    int result=sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );
    if (result !=SQLITE_OK){
        cerr<<errorMessage<<endl;
        return false;
    }
    return true;
}
Void Database::Close(){
    if(db){
        sqlite3_close(db);
        db=nullptr;
        cout<<"Database close"<<endl;
    }
}
sqlite3* Database::getConnection(){
    return db;
}