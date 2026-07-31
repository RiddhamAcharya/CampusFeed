#include "auth_controller.h"
crow::response AuthController::registerUser(const::request& req){
    auto body=crow::json::load(req.body);
    if(!body){
        return crow::response(
            400,
            R"({"Success":false,"message":"Invalid JSON."})"

        );

    }
} if(!body.has("full_name") ||
       !body.has("email") ||
       !body.has("password"))
    {
        return crow::response(
            400,
            R"({"success":false,"message":"Missing required fields."})"
        );
    }

    auto result = authService.registerUser(
        body["full_name"].s(),
        body["email"].s(),
        body["password"].s()
    ); crow::json::wvalue response;

    response["success"] = result.success;
    response["message"] = result.message;

    return crow::response(
        result.success ? 201 : 400,
        response
    );
}
