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

std::vector<Candlestick> ComputeCandlesticks::GetCandlesticks(
    std::string inputType,
    std::string symbol,
    std::string timeframe)
{
    auto orders = CSVReader::readCSV("20200601.csv");

    OrderBookType type = OrderBookEntry::stringToOrderBookType(inputType);

    // Vector is {open, high, low, close}
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
                std::vector<double> candle = buckets[key];

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

    for (std::__1::pair<const time_t, std::__1::vector<double>>& keyValue : buckets)
    {
        time_t key = keyValue.first;
        std::vector<double> candle = keyValue.second;

        result.emplace_back(
            TimeToString(key), // date
            candle[0], // open
            candle[1], // high
            candle[2], // low
            candle[3]  // close
        );
    }

    return result;
}


time_t ComputeCandlesticks::to_time_t(const std::string& ts)
{
    int y, mon, d, h, min, sec;
    long micro;

    char slash1, slash2, space, colon1, colon2, dot;

    std::stringstream ss(ts.substr(0, 26)); // cut at microsecond region

    ss >> y >> slash1 >> mon >> slash2 >> d
       >> space
       >> h >> colon1 >> min >> colon2 >> sec
       >> dot >> micro;

    std::tm t = {};
    t.tm_year = y - 1900;
    t.tm_mon  = mon - 1;
    t.tm_mday = d;
    t.tm_hour = h;
    t.tm_min  = min;
    t.tm_sec  = sec;

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
