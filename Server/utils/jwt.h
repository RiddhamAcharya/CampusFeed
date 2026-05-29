#pragma once

#include <string>

std::string create_token(int user_id);

bool verify_token(const std::string& token);

int get_user_id_from_token(const std::string& token);