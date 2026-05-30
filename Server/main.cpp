#include "crow.h"
#include "db/db.h"
#include "routes/auth_routes.h"
#include "routes/notification_routes.h"
#include "routes/interaction_routes.h" 
#include "routes/user_routes.h"
#include "middleware/auth_middleware.h"
#include "utils/app.h"

int main() {

    App app;

    Database db("../../../Database/campusfeed.db");
    db.loadAllSchemas("../../../Database/Schema/");

    db.execute(
    "CREATE TABLE IF NOT EXISTS notifications ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "user_id INTEGER NOT NULL,"
    "title TEXT NOT NULL,"
    "message TEXT NOT NULL,"
    "is_read INTEGER DEFAULT 0"
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

    setupNotificationRoutes(app, db);

    setupInteractionRoutes(app, db);

    setupUserRoutes(app, db);
    
    app.port(18080).multithreaded().run();
}
