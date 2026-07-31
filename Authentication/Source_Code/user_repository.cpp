#include "user_repository.h"
#include <iostream>

using namespace std;

UserRepository::UserRepository(Database& db)
    : database(db)
{
}

bool UserRepository::emailExists(const string& email)
{
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT id FROM users WHERE email = ? LIMIT 1;";

    int rc = sqlite3_prepare_v2(
        database.getConnection(),
        sql,
        -1,
        &stmt,
        nullptr
    );

    if (rc != SQLITE_OK)
    {
        cerr << "SQLite Prepare Error: "
             << sqlite3_errmsg(database.getConnection())
             << endl;

        return false;
    }

    sqlite3_bind_text(
        stmt,
        1,
        email.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = true;
    }

    sqlite3_finalize(stmt);

    return exists;
}

bool UserRepository::createUser(const User& user){
    sqlite3_stmt*stmt=nullptr;
    const char*sql=
    R"(INSERT INTO USERS(full_name,email,
    password_hash,role) value(?,?,?,?););
    
    if(sqlite3_prepare_v2(database.getConnection(),sql,-1,&stmt,nullptr)!=SQLITE_OK){
    return false;
}
    sqlite3_bind_text(stmt,1,user.fullName.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,1,user.email.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,1,user.passwordHash.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,1,user.role.c_str(),-1,SQLITE_TRANSIENT);
     
    bool success=sqlite3_step(stmt)==SQLITE_DONE;
    sqlite3_finalize(stmt);
    return Success;
}