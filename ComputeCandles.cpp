#include "Candlestick.h"
#include "ComputeCandles.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <vector>

ComputeCandlesticks::ComputeCandlesticks()
{

}

Candlestick ComputeCandlesticks::GetCandlesticks(std::string inputType, std::string input){
    std::vector<OrderBookEntry> orders;
    orders = CSVReader::readCSV("20200601.csv");

    // Shortening the timestamp to just get the date
    std::string first_date = orders[0].timestamp.substr(0,10);

    // Vector of just BTC/USDT pairs
    std::vector<OrderBookEntry> PairOrders;

    OrderBookType type = OrderBookEntry::stringToOrderBookType(inputType);
    
    // Pushing all the BTCUSDT pair order book entries into the vector
    for(const OrderBookEntry& entry : orders){
        if(first_date == entry.timestamp.substr(0,10)){
            if(entry.product == input && entry.orderType == type){
                PairOrders.push_back(entry);
            }
        }
    }

    // Setting the default values to the first value
    double open_price = PairOrders[0].price;
    double close_price;
    double max_price = PairOrders[0].price;
    double min_price = PairOrders[0].price;

    // Getting the max and min prices
    for(const OrderBookEntry& entry : PairOrders){
        if(entry.price > max_price){
            max_price = entry.price;
        }
        if(entry.price < min_price){
            min_price = entry.price;
        }
        close_price = entry.price;
    }

    Candlestick Candle(orders[0].timestamp, open_price, max_price, min_price, close_price);
    return Candle;
}

