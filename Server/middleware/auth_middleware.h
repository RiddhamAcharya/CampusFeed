#pragma once

#include <crow.h>
#include <string>

#include "../utils/jwt.h"

struct AuthMiddleware
{
    struct context
    {
        int user_id = -1;
    };

    void before_handle(crow::request& req,
                       crow::response& res,
                       context& ctx)
    {
        // Public routes
        if (req.url == "/login" || req.url == "/signup")
        {
            return;
        }

        std::string auth_header =
            req.get_header_value("Authorization");

        if (auth_header.empty())
        {
            res.code = 401;
            res.write("Missing Authorization header");
            res.end();
            return;
        }

        const std::string bearer = "Bearer ";

        // Must start with "Bearer "
        if (auth_header.find(bearer) != 0)
        {
            res.code = 401;
            res.write("Invalid token format");
            res.end();
            return;
        }

        // Extract token
        std::string token =
            auth_header.substr(bearer.length());

        // Verify token
        if (!verify_token(token))
        {
            res.code = 401;
            res.write("Invalid or expired token");
            res.end();
            return;
        }

        // Save user_id into middleware context
        ctx.user_id = get_user_id_from_token(token);
    }

    void after_handle(crow::request& req,
                      crow::response& res,
                      context& ctx)
    {
        // Nothing needed for now
    }
};