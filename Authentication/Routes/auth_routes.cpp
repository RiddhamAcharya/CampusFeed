#include <crow.h>

#include "auth_controller.h"

void registerAuthRoutes(
    crow::SimpleApp& app,
    AuthController& controller)
{
    CROW_ROUTE(app, "/api/auth/register")
    .methods(crow::HTTPMethod::POST)
    ([&controller](const crow::request& req)
    {
        return controller.registerUser(req);
    });
}
#include "auth_routes.h"

void registerAuthRoutes(
    crow::SimpleApp& app,
    AuthController& controller)
{
    CROW_ROUTE(app, "/api/auth/register")
        .methods(crow::HTTPMethod::POST)
        ([&controller](const crow::request& req)
    {
        return controller.registerUser(req);
    });

    CROW_ROUTE(app, "/api/auth/login")
        .methods(crow::HTTPMethod::POST)
        ([&controller](const crow::request& req)
    {
        return controller.loginUser(req);
    });
}