cmake_minimum_required(VERSION 3.10)

project(CampusFeedServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Crow CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(server
    main.cpp
    db/db.cpp
    routes/auth_routes.cpp
    routes/notification_routes.cpp
    utils/hash.cpp
    utils/jwt.cpp
)

target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(server PRIVATE
    Crow::Crow
    unofficial::sqlite3::sqlite3
    OpenSSL::SSL
    OpenSSL::Crypto
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
)