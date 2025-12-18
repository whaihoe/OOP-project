#include "User.h"
#include <sstream>

User::User(
    const std::string& username,
    const std::string& fullName,
    const std::string& email,
    size_t passwordHash
)
    : username(username),
      fullName(fullName),
      email(email),
      passwordHash(passwordHash)
{
}

std::string User::getUsername() const
{
    return username; 
}
std::string User::getFullName() const
{
    return fullName; 
}
std::string User::getEmail() const
{ 
    return email;
}
size_t User::getPasswordHash() const
{
    return passwordHash; 
}

std::string User::toCSV() const
{
    std::ostringstream oss;
    oss << username << ","
        << fullName << ","
        << email << ","
        << passwordHash;
    return oss.str();
}

User User::fromCSV(const std::string& line)
{
    std::stringstream ss(line);
    std::string username, fullName, email, hashStr;

    std::getline(ss, username, ',');
    std::getline(ss, fullName, ',');
    std::getline(ss, email, ',');
    std::getline(ss, hashStr, ',');

    size_t passwordHash = std::stoull(hashStr);
    return User(username, fullName, email, passwordHash);
}

void User::setPasswordHash(size_t newHash){
    passwordHash = newHash;
}



