#include "Candlestick.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "ComputeCandles.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>


Candlestick::Candlestick(std::string _date,
                        double _open,
                        double _high,
                        double _low,
                        double _close)
: date(_date),
open(_open),
high(_high),
low(_low),
close(_close) 
{

}

int main(){

    std::cout << "Enter a pair: ";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;

    // Extract words one by one using the >> operator
    while (ss >> word) {
        words.push_back(word);
    }

    std::vector<Candlestick> candles = ComputeCandlesticks::GetCandlesticks(words[0], words[1], words[2]);

    // for(const Candlestick& candle : candles){
    //     std::cout << candle.date << std::endl;
    //     std::cout << line << std::endl;
    //     std::cout << "Max price: " << candle.high << std::endl;
    //     std::cout << "Min price: " << candle.low << std::endl;
    //     std::cout << "Open price: " << candle.open << std::endl;
    //     std::cout << "Close price: " << candle.close << std::endl;
    // }

    std::cout << words[2] << " " << words[0] << " Candlesticks for " << words[1] << std::endl;

    std::cout 
    << std::left  << std::setw(20) << "Date"
    << std::right << std::setw(10) << "Open"
    << std::setw(10) << "High"
    << std::setw(10) << "Low"
    << std::setw(10) << "Close"
    << std::endl;

    for (const auto& candle : candles)
    {
        std::cout
            << std::left  << std::setw(20) << candle.date
            << std::right << std::setw(10) << candle.open
            << std::setw(10) << candle.high
            << std::setw(10) << candle.low
            << std::setw(10) << candle.close
            << std::endl;
    }


}