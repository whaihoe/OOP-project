#include "UserManager.h"
#include "User.h"
#include <iostream>
#include <fstream>

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
        users.push_back(User::fromCSV(line));
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
    const std::string& username,
    const std::string& password
)
{   
    if (usernameExists(username)) {
        std::cout << "Username already taken." << std::endl;
        return false;
    }

    if (userExists(fullName, email)) {
        std::cout << "User already registered." << std::endl;
        return false;
    }

    size_t passwordHash = hashPassword(password);
    User user(username, fullName, email, passwordHash);
    users.push_back(user);
    saveUser(user);
    std::cout << "Account successfully created." << std::endl;
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

int main(){
    UserManager manager;

    std::string fullname;
    std::string email;
    std::string username;
    std::string password;

    std::cout << "1. Register" << std::endl << "2. Login" << std::endl;
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
        
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);

        std::cout << "Enter your password: ";
        std::getline(std::cin, password);

        manager.registerUser(fullname, email, username, password);
    }
    
    if(input == 2)
    {
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);

        std::cout << "Enter your password: ";
        std::getline(std::cin, password);

        auto loggedInUser = manager.login(username, password);
        if (loggedInUser) {
            std::cout << "Welcome " << loggedInUser->getUsername() << std::endl;
        } else {
            std::cout << "Error logging in. Please try again.\n";
        }
    }
}