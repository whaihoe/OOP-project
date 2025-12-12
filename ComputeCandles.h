#include "Candlestick.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <vector>

class ComputeCandlesticks
{
    public:
        ComputeCandlesticks();

        static Candlestick GetCandlesticks(std::string inputType, std::string input);
};