#include "ComputeCandles.h"
#include "MerkelMain.h"
#include "DateTime.h"
#include <iostream>
#include <sstream>
#include <iomanip>

ComputeCandlesticks::ComputeCandlesticks() {}

std::vector<Candlestick> ComputeCandlesticks::GetCandlesticks(const std::string& CurrentTime)
{
    std::cout << "Enter a pair e.g. bid BTC/USDT 1h" << std::endl;
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;

    while (ss >> word) words.push_back(word);

    if (words.size() < 3) {
        std::cout << "Invalid input!" << std::endl;
        return {};
    }

    std::string inputType = words[0];
    std::string symbol    = words[1];
    std::string timeframe = words[2];

    auto orders = CSVReader::readCSV("20200601.csv");
    OrderBookType type = OrderBookEntry::stringToOrderBookType(inputType);

    // get current time
    DateTime simulatedNow = DateTime::fromString(CurrentTime);

    // Map with string key (bucketed datetime) -> {open, high, low, close}
    std::map<std::string, std::vector<double>> buckets;

    for (const OrderBookEntry& entry : orders)
    {
        if (entry.product == symbol && entry.orderType == type)
        {
            DateTime dt = DateTime::fromString(entry.timestamp);

            // To skip the timeframes after current timestamp
            if (dt.isAfter(simulatedNow)) continue;

            DateTime bucket = dt.toBucket(timeframe);
            std::string key = bucket.toString();

            double price = entry.price;

            if (buckets.find(key) == buckets.end()) {
                buckets[key] = {price, price, price, price}; // open, high, low, close
            } else {
                std::vector<double>& candle = buckets[key];
                if (price > candle[1]) candle[1] = price; // high
                if (price < candle[2]) candle[2] = price; // low
                candle[3] = price; // close
            }
        }
    }

    // Convert to final vector of Candlesticks
    std::vector<Candlestick> result;
    for (auto& kv : buckets) {
        const std::string& key = kv.first;
        std::vector<double>& candle = kv.second;
        
        result.emplace_back(
            key,        // date as string
            candle[0],  // open
            candle[1],  // high
            candle[2],  // low
            candle[3]   // close
        );
    }

    // Print
    std::cout << std::endl << timeframe << " " << inputType << " Candlesticks for " << symbol << std::endl;
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
