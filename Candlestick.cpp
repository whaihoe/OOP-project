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

// int main(){
//     std::vector<Candlestick> candles = ComputeCandlesticks::GetCandlesticks();
// }