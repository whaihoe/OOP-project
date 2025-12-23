#pragma once
#include "User.h"
#include <vector>

class UserManager {
public:
    UserManager();

    // Show login/register/forgot password menu
    void showAuthMenu();
    bool isLoggedIn() const;
    // gets logged-in user
    User* getCurrentUser(); 

    User* login(
        const std::string& username,
        const std::string& password
    );

    bool registerUser(
        const std::string& fullName,
        const std::string& email,
        const std::string& password
    );

    bool resetPassword();

private:
    // Logged in user
    User* currentUser = nullptr;

    void loadUsers();
    void saveUser(const User& user);
    std::vector<User> users;
    size_t hashPassword(const std::string& password);
    bool userExists(const std::string& fullName, const std::string& email);
    bool usernameExists(const std::string& username);
    std::string generateUsername();
};
