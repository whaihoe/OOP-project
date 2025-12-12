#include "Candlestick.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "ComputeCandles.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


Candlestick::Candlestick(std::string _date,
                        double _open,
                        double _high,
                        double _low,
                        double _close)
: open(_open),
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

    Candlestick candle = ComputeCandlesticks::GetCandlesticks(words[0], words[1]);

    std::cout << candle.date << std::endl;
    std::cout << line << std::endl;
    std::cout << "Max price: " << candle.high << std::endl;
    std::cout << "Min price: " << candle.low << std::endl;
    std::cout << "Open price: " << candle.open << std::endl;
    std::cout << "Close price: " << candle.close << std::endl;

}