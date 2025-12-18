#include "UserManager.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <random>

UserManager::UserManager()
{
    loadUsers();
}


void UserManager::loadUsers()
{
    users.clear();

    std::ifstream file("users.csv");
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) {
            continue; // 👈 CRITICAL
        }

        try {
            users.push_back(User::fromCSV(line));
        }
        catch (const std::exception& e) {
            std::cerr << "Skipping invalid CSV line: " << e.what() << std::endl;
        }
    }
}

void UserManager::saveUser(const User& user)
{
    std::ofstream file("users.csv", std::ios::app);
    file << user.toCSV() << std::endl;
}

User* UserManager::login(const std::string& username, const std::string& password)
{
    size_t inputHash = hashPassword(password);

    for (auto& user : users)
    {
        if (user.getUsername() == username &&
            user.getPasswordHash() == inputHash)
        {
            return &user;
        }
    }

    return nullptr;
}

size_t UserManager::hashPassword(const std::string& password)
{
    return std::hash<std::string>{}(password);
}

bool UserManager::registerUser(
    const std::string& fullName,
    const std::string& email,
    const std::string& password
)
{
    if (userExists(fullName, email)) {
        std::cout << "User already registered.\n";
        return false;
    }

    std::string username = generateUsername();
    size_t passwordHash = hashPassword(password);

    User user(username, fullName, email, passwordHash);
    users.push_back(user);
    saveUser(user);

    std::cout << "Account successfully created!" << std::endl ;
    std::cout << "Your generated username is: " << username << std::endl;
    std::cout << "Please keep it safe for login." << std::endl;

    return true;
}


bool UserManager::userExists(const std::string& fullName, const std::string& email)
{
    for (const auto& user : users)
    {
        if (user.getFullName() == fullName &&
            user.getEmail() == email)
            return true;
    }
    return false;
}

bool UserManager::usernameExists(const std::string& username) 
{
    for (const User& u : users) {
        if (u.getUsername() == username)
            return true;
    }
    return false;
}

std::string UserManager::generateUsername()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dist(1000000000LL, 9999999999LL);

    std::string username;

    do {
        username = std::to_string(dist(gen));
    } while (usernameExists(username));

    return username;
}

bool UserManager::resetPassword(
    const std::string& username,
    const std::string& email,
    const std::string& newPassword,
    const std::string& confirmPassword
)
{
    if (newPassword != confirmPassword) {
        std::cout << "Passwords do not match." << std::endl;
        return false;
    }

    for (auto& user : users)
    {
        if (user.getUsername() == username &&
            user.getEmail() == email)
        {
            size_t newHash = hashPassword(newPassword);

            user.setPasswordHash(newHash);

            // Rewrite entire CSV
            std::ofstream file("users.csv", std::ios::trunc);
            for (const auto& u : users) {
                file << u.toCSV() << std::endl;
            }

            std::cout << "Password reset successful." << std::endl;
            return true;
        }
    }

    std::cout << "Username and email do not match." << std::endl;
    return false;
}



int main(){
    UserManager manager;

    std::string fullname;
    std::string email;
    std::string username;
    std::string password;

    std::cout << "1. Register" << std::endl << "2. Login" << std::endl << "3. Forget password" << std::endl;
    std::cout << "Enter your choice: ";

    int input;
    std::cin >> input;
    std::cin.ignore(); 

    if(input == 1)
    {
        std::cout << "Enter your name: ";
        std::getline(std::cin, fullname);

        std::cout << "Enter your email: ";
        std::getline(std::cin, email);

        std::cout << "Enter your password: ";
        std::getline(std::cin, password);

        manager.registerUser(fullname, email, password);
    }
    
    else if(input == 2)
    {
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);

        std::cout << "Enter your password: ";
        std::getline(std::cin, password);

        auto loggedInUser = manager.login(username, password);
        if (loggedInUser) {
            std::cout << "Welcome " << loggedInUser->getFullName() << std::endl;
        } else {
            std::cout << "Error logging in. Please try again." << std::endl;
        }
    }
    else if(input == 3)
    {
        std::string username, email, newPass, confirmPass;

        std::cout << "Enter your username: ";
        std::getline(std::cin, username);

        std::cout << "Enter your email: ";
        std::getline(std::cin, email);

        std::cout << "Enter new password: ";
        std::getline(std::cin, newPass);

        std::cout << "Confirm new password: ";
        std::getline(std::cin, confirmPass);

        manager.resetPassword(username, email, newPass, confirmPass);
    }
}