#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "User.h"
#include "TransactionLogger.h"


class MerkelMain
{
    public:
        MerkelMain(User& user);
        /** Call this to start the sim */
        void init();
    private: 
        User& user; 
        TransactionLogger transactionLogger;
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        void depositFunds();
        void withdrawFunds();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;

//        OrderBook orderBook{"20200317.csv"};
        OrderBook orderBook{"20200601.csv"};
        Wallet wallet;

};
