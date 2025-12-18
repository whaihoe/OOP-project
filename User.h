#pragma once
#include <string>

class User
{
    friend class UserManager;
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
    void updatePasswordHash(size_t newHash);

private:
    // Unique Username
    std::string username;
    std::string fullName;
    std::string email;
    size_t passwordHash;      // hashed password
    void setPasswordHash(size_t newHash);
};
