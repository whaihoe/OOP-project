#include "DateTime.h"
#include <sstream>
#include <iostream>
#include <iomanip>

DateTime DateTime::fromString(const std::string& timestamp)
{
    DateTime dt{};

    // Extract each part using substr and convert to int
    dt.year   = std::stoi(timestamp.substr(0, 4));   // YYYY
    dt.month  = std::stoi(timestamp.substr(5, 2));   // MM
    dt.day    = std::stoi(timestamp.substr(8, 2));   // DD
    dt.hour   = std::stoi(timestamp.substr(11, 2));  // HH
    dt.minute = std::stoi(timestamp.substr(14, 2));  // MM
    dt.second = std::stoi(timestamp.substr(17, 2));  // SS

    return dt;
}


std::string DateTime::toString() const
{
    std::ostringstream oss;

    oss << std::setw(4) << std::setfill('0') << year << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << std::setw(2) << std::setfill('0') << day << " "
        << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute << ":"
        << std::setw(2) << std::setfill('0') << second;

    return oss.str();
}

DateTime DateTime::toBucket(const std::string& timeframe) const
{
    DateTime bucket = *this;

    if (timeframe == "5m") {
        bucket.minute = (bucket.minute / 5) * 5;
        bucket.second = 0;
    }
    else if (timeframe == "1h") {
        bucket.minute = 0;
        bucket.second = 0;
    }
    else if (timeframe == "1d") {
        bucket.hour = 0;
        bucket.minute = 0;
        bucket.second = 0;
    }
    else if (timeframe == "1M") {
        bucket.day = 1;
        bucket.hour = 0;
        bucket.minute = 0;
        bucket.second = 0;
    }
    else if (timeframe == "1Y") {
        bucket.month = 1;
        bucket.day = 1;
        bucket.hour = 0;
        bucket.minute = 0;
        bucket.second = 0;
    }

    return bucket;
}

bool DateTime::sameBucket(const DateTime& other) const
{
    return year   == other.year &&
           month  == other.month &&
           day    == other.day &&
           hour   == other.hour &&
           minute == other.minute;
}

// int main(){

//     DateTime dt  = DateTime::fromString("2020/06/01 14:56:35.210165"); 
//     std::cout << dt.toString() << std::endl;
//     DateTime bucket = dt.toBucket("1h");
//     std::cout << bucket.toString() << std::endl;
// }