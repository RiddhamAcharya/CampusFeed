#pragma once
#include "auth_service.h"
#include <crow.h>
class AuthController{
    private:
    AuthService& authService;
    public:
    AuthController(AuthService& service);
    crow::response registerUser(const crow::request& req);
    crow::response loginUser(const crow::request& req);

};
