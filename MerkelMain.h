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
        // To pass logged in user in Task 2
        MerkelMain(User& user);
        /** Call this to start the sim */
        void init();
    private: 

        // User object in Task 2
        User& user; 
        TransactionLogger transactionLogger;
        void printMenu();
        void printHelp();
        void printMarketStats();

        // For Task 4 to generate 5 bids and 5 asks
        void generateBidsAndAsks();
        // To get the reference price of the products
        double getReferencePrice(const std::string& product);

        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();

        // To deposit and withdraw money for Task 3
        void modifyFunds();

        // For Task 3 user statistics
        void printSummary();
        int getUserOption();
        bool processUserOption(int userOption);

        std::string currentTime;

//        OrderBook orderBook{"20200317.csv"};
        OrderBook orderBook{"20200601.csv"};
        Wallet wallet;
};
