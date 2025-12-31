#include "UserManager.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <random>
#include <termios.h>
#include <unistd.h>

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
            continue;
        }

        try {
            users.push_back(User::fromCSV(line));
        }
        catch (const std::exception& e) {

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
    if (username.empty() || password.empty())
    {
        std::cout << "Invalid input: username or password is empty." << std::endl;
        return nullptr;
    }

    size_t inputHash = hashPassword(password);

    for (auto& user : users)
    {
        if (user.getUsername() == username &&
            user.getPasswordHash() == inputHash)
        {
            currentUser = &user;
            return currentUser;
        }
    }

    std::cout << "Invalid username or password." << std::endl;
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
    if (fullName.empty() || email.empty() || password.empty())
    {
        std::cout << "Invalid input: all fields are required." << std::endl;
        return false;
    }

    if (password.length() < 6)
    {
        std::cout << "Password must be at least 6 characters." << std::endl;
        return false;
    }

    if (userExists(fullName, email))
    {
        std::cout << "User already registered." << std::endl;
        return false;
    }

    std::string username = generateUsername();
    size_t passwordHash = hashPassword(password);

    User user(username, fullName, email, passwordHash);
    users.push_back(user);
    saveUser(user);

    std::cout << "Account successfully created!" << std::endl;
    std::cout << "Your generated username is: " << username << std::endl;

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

bool UserManager::resetPassword()
{
    std::string username, email, newPassword, confirmPassword;

    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    std::cout << "Enter your email: ";
    std::getline(std::cin, email);

    // For password censor
    char ch;

    // Disable terminal echo
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cout << "Enter new password: ";
    while (std::cin.get(ch) && ch != '\n') {
        if (ch == 127 || ch == 8) { // Handle backspace
            if (!newPassword.empty()) {
                newPassword.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else {
            newPassword.push_back(ch);
            std::cout << '*' << std::flush;
        }
    }

    std::cout << std::endl << "Confirm new password: ";
    while (std::cin.get(ch) && ch != '\n') {
        if (ch == 127 || ch == 8) {
            if (!confirmPassword.empty()) {
                confirmPassword.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else {
            confirmPassword.push_back(ch);
            std::cout << '*' << std::flush;
        }
    }

    if (username.empty() || email.empty() ||
        newPassword.empty() || confirmPassword.empty())
    {
        std::cout << "Invalid input: fields cannot be empty." << std::endl;
        return false;
    }

    if (newPassword != confirmPassword)
    {
        std::cout << "Passwords do not match." << std::endl;
        return false;
    }



    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    // std::cout << "Confirm new password: ";
    // std::getline(std::cin, confirmPassword);



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

bool UserManager::isLoggedIn() const
{
    return currentUser != nullptr;
}

User* UserManager::getCurrentUser()
{
    return currentUser;
}

void UserManager::showAuthMenu()
{
    while (!isLoggedIn())
    {
        std::cout << "==== Welcome ====" << std::endl;
        std::cout << "1. Register" << std::endl;
        std::cout << "2. Login" << std::endl;
        std::cout << "3. Forgot Password" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choice: ";

        std::string input;
        std::getline(std::cin, input);

        // Check if input is empty
        if (input.empty()) {
            std::cout << "Invalid input! Please enter a number 0-3." << std::endl;
            continue;
        }

        // Try converting to int
        int choice;
        try {
            choice = std::stoi(input); // throws if not a valid integer
        } catch (...) {
            std::cout << "Invalid input! Please enter a number 0-3." << std::endl;
            continue;
        }

        // Check range
        if (choice < 0 || choice > 3) {
            std::cout << "Invalid choice! Please enter a number 0-3." << std::endl;
            continue;
        }

        if (choice == 1)
        {
            std::string name, email, password;
            std::cout << "Full name: ";
            std::getline(std::cin, name);
            if (name.empty()) {
                std::cout << "Name cannot be empty!" << std::endl;
                continue;
            }

            std::cout << "Email: ";
            std::getline(std::cin, email);
            if (email.empty() || email.find('@') == std::string::npos) {
                std::cout << "Invalid email!" << std::endl;
                continue;
            }

            // Password input with masking
            char ch;
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            termios newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            std::cout << "Password: ";
            password.clear();
            while (std::cin.get(ch) && ch != '\n') {
                if (ch == 127 || ch == 8) {
                    if (!password.empty()) {
                        password.pop_back();
                        std::cout << "\b \b" << std::flush;
                    }
                } else {
                    password.push_back(ch);
                    std::cout << '*' << std::flush;
                }
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            std::cout << std::endl;

            if (password.empty()) {
                std::cout << "Password cannot be empty!" << std::endl;
                continue;
            }

            registerUser(name, email, password);
        }
        else if (choice == 2)
        {
            std::string username, password;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            if (username.empty()) {
                std::cout << "Username cannot be empty!" << std::endl;
                continue;
            }

            char ch;
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            termios newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            std::cout << "Password: ";
            password.clear();
            while (std::cin.get(ch) && ch != '\n') {
                if (ch == 127 || ch == 8) {
                    if (!password.empty()) {
                        password.pop_back();
                        std::cout << "\b \b" << std::flush;
                    }
                } else {
                    password.push_back(ch);
                    std::cout << '*' << std::flush;
                }
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            std::cout << std::endl;

            if (password.empty()) {
                std::cout << "Password cannot be empty!" << std::endl;
                continue;
            }

            if (!login(username, password))
                std::cout << "Invalid credentials." << std::endl;
        }
        else if (choice == 3)
        {
            resetPassword();
        }
        else if (choice == 0)
        {
            std::exit(0);
        }
        else
        {
            std::cout << "Invalid choice! Please enter 0-3." << std::endl;
        }
    }
}




// int main(){
//     UserManager manager;

//     manager.showAuthMenu();
// }


