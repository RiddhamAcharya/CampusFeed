#include "jwt.h"

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>

const std::string SECRET_KEY = "campusfeed_secret_key";

std::string create_token(int user_id)
{
    auto token = jwt::create<jwt::traits::nlohmann_json>()
        .set_issuer("campusfeed")
        .set_type("JWT")
        .set_payload_claim(
            "user_id",
            jwt::basic_claim<jwt::traits::nlohmann_json>(
                std::to_string(user_id)
            )
        )
        .sign(jwt::algorithm::hs256{SECRET_KEY});

    return token;
}

bool verify_token(const std::string& token)
{
    try
    {
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);

        auto verifier = jwt::verify<jwt::traits::nlohmann_json>()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
            .with_issuer("campusfeed");

        verifier.verify(decoded);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

int get_user_id_from_token(const std::string& token)
{
    auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);

    auto claim = decoded.get_payload_claim("user_id");

    return std::stoi(claim.as_string());
}