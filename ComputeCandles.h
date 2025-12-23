#include "Candlestick.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <vector>

class ComputeCandlesticks
{
    public:
        ComputeCandlesticks();

        static std::vector<Candlestick> GetCandlesticks(const std::string& CurrentTime);

    private:
        static time_t to_time_t(const std::string& ts);
        static time_t bucket_time(time_t t, const std::string& tf);
        static std::string TimeToString(time_t t);
};