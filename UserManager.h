#pragma once
#include "User.h"
#include <vector>

class UserManager {
public:
    UserManager();
    User* login(
        const std::string& username,
        const std::string& password
    );

    bool registerUser(
        const std::string& fullName,
        const std::string& email,
        const std::string& username,
        const std::string& password
    );

private:
    void loadUsers();
    void saveUser(const User& user);
    std::vector<User> users;
    size_t hashPassword(const std::string& password);
    bool userExists(const std::string& fullName, const std::string& email);
    bool usernameExists(const std::string& username);
};
