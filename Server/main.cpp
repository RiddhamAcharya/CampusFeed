#include "crow.h"
#include "db/db.h"
#include "routes/auth_routes.h"

int main() {

    crow::SimpleApp app;

    Database db("../../../Database/campusfeed.db");

    db.execute(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL"
        ");"
    );

    CROW_ROUTE(app, "/health")([]() {
        return "CampusFeed server alive";
    });
    
    CROW_ROUTE(app, "/db-test")([&]() {
        db.execute("SELECT 1;");
        return "DB OK";
    });

    setupAuthRoutes(app, db);

    app.port(18080).multithreaded().run();
}