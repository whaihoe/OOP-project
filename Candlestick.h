#pragma once

#include "OrderBook.h"
#include "CSVReader.h"
#include "MerkelMain.h"
#include <vector>
#include <string>


class Candlestick
{
    public:
    
        Candlestick(
            std::string _date,
            double _open,
            double _high,
            double _low,
            double _close);
        
            std::string date;
            double open;
            double high;
            double low;
            double close;

    private:
            
};