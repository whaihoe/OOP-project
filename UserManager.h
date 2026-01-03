// For login, registration, reset password, saving user to CSV and printing login menu for Task 2

#pragma once
#include "User.h"
#include <vector>

class UserManager {
public:
    UserManager();

    // Show login/register/forgot password menu
    void showAuthMenu();
    bool isLoggedIn() const;

    // Gets logged-in user
    User* getCurrentUser(); 

    // Login function
    User* login(
        const std::string& username,
        const std::string& password
    );

    // Register function
    bool registerUser(
        const std::string& fullName,
        const std::string& email,
        const std::string& password
    );

    // Reset Password function
    bool resetPassword();

private:
    // Logged in user
    User* currentUser = nullptr;

    // Read users from CSV file
    void loadUsers();

    // Save user to CSV file
    void saveUser(const User& user);
    std::vector<User> users;

    // To hash the password to make it safe to store
    size_t hashPassword(const std::string& password);
    bool userExists(const std::string& fullName, const std::string& email);
    bool usernameExists(const std::string& username);

    // Generate 10 digit unique username
    std::string generateUsername();
};
