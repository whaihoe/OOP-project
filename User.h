#pragma once
#include <string>

class User
{
public:
    User() = default;

    User(
        const std::string& username,
        const std::string& fullName,
        const std::string& email,
        size_t passwordHash
    );

    std::string getUsername() const;
    std::string getFullName() const;
    std::string getEmail() const;
    size_t getPasswordHash() const;

    std::string toCSV() const;
    static User fromCSV(const std::string& line);

private:
    // Unique Username
    std::string username;
    std::string fullName;
    std::string email;
    size_t passwordHash;      // hashed password
};
