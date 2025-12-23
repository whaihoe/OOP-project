#pragma once

#include <string>

struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    // Parse from "YYYY/MM/DD HH:MM:SS.xxx"
    static DateTime fromString(const std::string& timestamp);

    // Convert to string "YYYY/MM/DD HH:MM:SS"
    std::string toString() const;

    // Normalize to bucket start based on timeframe
    DateTime toBucket(const std::string& timeframe) const;

    // Check if two DateTimes are the same bucket
    bool sameBucket(const DateTime& other) const;
};
