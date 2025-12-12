#include "Candlestick.h"
#include "ComputeCandles.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

ComputeCandlesticks::ComputeCandlesticks()
{

}

std::vector<Candlestick> ComputeCandlesticks::GetCandlesticks()
{

    std::cout << "Enter a pair e.g. bid BTC/USDT 1h" << std::endl;
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;

    // Extract words one by one using the >> operator
    while (ss >> word) {
        words.push_back(word);
    }

    std::string inputType = words[0];
    std::string symbol = words[1];
    std::string timeframe = words[2];
    
    auto orders = CSVReader::readCSV("20200601.csv");

    OrderBookType type = OrderBookEntry::stringToOrderBookType(inputType);

    std::map<time_t, std::vector<double>> buckets;

    for (const OrderBookEntry& entry : orders)
    {
        if (entry.product == symbol && entry.orderType == type){
            // Parse date
            time_t date = to_time_t(entry.timestamp);

            // Using bucket function to get the orders in the timeframes
            time_t key = bucket_time(date, timeframe);

            double price = entry.price;

            // If bucket does not exist yet, create and initialize
            if (buckets.find(key) == buckets.end())
            {
                buckets[key] = {price, price, price, price};
            }
            else
            {
                std::vector<double>& candle = buckets[key];

                // Update high & low
                if (price > candle[1]) candle[1] = price;
                if (price < candle[2]) candle[2] = price;

                // Close always updates
                candle[3] = price;
            }
        }

    }

    // Convert map to final vector
    std::vector<Candlestick> result;

    for (std::pair<const time_t, std::__1::vector<double>>& keyValue : buckets)
    {
        time_t key = keyValue.first;
        std::vector<double>& candle = keyValue.second;

        result.emplace_back(
            TimeToString(key), // date
            candle[0], // open
            candle[1], // high
            candle[2], // low
            candle[3]  // close
        );
    }

    std::cout << words[2] << " " << words[0] << " Candlesticks for " << words[1] << std::endl;

    std::cout 
    << std::left  << std::setw(20) << "Date"
    << std::right << std::setw(10) << "Open"
    << std::setw(10) << "High"
    << std::setw(10) << "Low"
    << std::setw(10) << "Close"
    << std::endl;

    for (const auto& candle : result)
    {
        std::cout
            << std::left  << std::setw(20) << candle.date
            << std::right << std::setw(10) << candle.open
            << std::setw(10) << candle.high
            << std::setw(10) << candle.low
            << std::setw(10) << candle.close
            << std::endl;
    }

    return result;
}


time_t ComputeCandlesticks::to_time_t(const std::string& ts)
{
    int year, month, day, h, m, s;
    long micro;

    char slash1, slash2, space, colon1, colon2, dot;

    std::stringstream ss(ts.substr(0, 26)); // cut at microsecond region

    ss >> year >> slash1 >> month >> slash2 >> day
       >> space
       >> h >> colon1 >> m >> colon2 >> s
       >> dot >> micro;

    std::tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = h;
    t.tm_min  = m;
    t.tm_sec  = s;

    return std::mktime(&t); // local time
}

time_t ComputeCandlesticks::bucket_time(time_t t, const std::string& tf)
{
    std::tm* tm = std::localtime(&t);

    if (tf == "5m") {
        tm->tm_min = (tm->tm_min / 5) * 5;
        tm->tm_sec = 0;
    }
    else if (tf == "1h") {
        tm->tm_min = 0;
        tm->tm_sec = 0;
    }
    else if (tf == "1d") {
        tm->tm_hour = 0;
        tm->tm_min = 0;
        tm->tm_sec = 0;
    }
    else if (tf == "1M") {
        tm->tm_mday = 1;
        tm->tm_hour = 0;
        tm->tm_min = 0;
        tm->tm_sec = 0;
    }
    else if (tf == "1Y") {
        tm->tm_mon = 0;
        tm->tm_mday = 1;
        tm->tm_hour = 0;
        tm->tm_min = 0;
        tm->tm_sec = 0;
    }

    return std::mktime(tm);
}

std::string ComputeCandlesticks::TimeToString(time_t t) 
{
    std::tm tm = *std::localtime(&t);
    tm.tm_hour += 10;

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S"); // format: YYYY/MM/DD HH:MM:SS
    return oss.str();
}
