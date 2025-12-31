#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "ComputeCandles.h"
#include "DateTime.h"

MerkelMain::MerkelMain(User& user)
    : user(user),
    transactionLogger(user.getUsername()),
    wallet(user.getUsername())
{

}


void MerkelMain::init()
{
    int input;
    std::string lastTxTime = TransactionLogger::getLastTransactionTime(user.getUsername());
    if (!lastTxTime.empty()) {
        currentTime = lastTxTime;
        std::cout << "Resuming from last transaction timestamp: " << currentTime << std::endl;
    } else {
        currentTime = orderBook.getEarliestTime();
        std::cout << "No previous transactions, starting from earliest dataset time: " << currentTime << std::endl;
    }

    bool running = true;

    while(running)
    {
        std::cout << std::endl << "==== Welcome, " << user.getFullName() << " ===="<< std::endl;

        printMenu();
        input = getUserOption();
        running = processUserOption(input); // returns false if userOption == 0
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 generate 5 bids and asks
    std::cout << "3: Generate bids and asks" << std::endl;
    // 4 make an offer
    std::cout << "4: Make an offer " << std::endl;
    // 5 make a bid 
    std::cout << "5: Make a bid " << std::endl;
    // 6 print wallet
    std::cout << "6: Print wallet " << std::endl;
    // 7 print candlestickdata
    std::cout << "7: Print candlestick " << std::endl;
    // 8 deposit into wallet   
    std::cout << "8: Deposit / withdraw " << std::endl;
    // 9 withdraw from wallet
    std::cout << "9: Print user summary " << std::endl;
    // 10 continue to next timeframe
    std::cout << "10: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> AskEntries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << AskEntries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(AskEntries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(AskEntries) << std::endl;
        
        // Bids
        std::vector<OrderBookEntry> BidEntries = orderBook.getOrders(OrderBookType::bid, 
                                                                p, currentTime);
        std::cout << "Bids seen: " << BidEntries.size() << std::endl;
        std::cout << "Max bid: " << OrderBook::getHighPrice(BidEntries) << std::endl;
        std::cout << "Min bid: " << OrderBook::getLowPrice(BidEntries) << std::endl << std::endl;


    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}



void MerkelMain::generateBidsAndAsks()
{
    for (const std::string& product : orderBook.getKnownProducts())
    {
        std::vector<std::string> currs = CSVReader::tokenise(product, '/');
        std::string base = currs[0];
        std::string quote = currs[1];

        double bestAsk = orderBook.getMinBidPrice(product, currentTime);
        double bestBid = orderBook.getMaxAskPrice(product, currentTime);

        if (bestAsk <= 0 || bestBid <= 0)
            continue;

        // --------------------
        // ASKS (sell base)
        // --------------------
        double baseBalance = wallet.getAmount(base);
        if (baseBalance == 0.0)
        {
            wallet.deposit(base, 10);
            transactionLogger.log(currentTime, 0.0, 10, base, "deposit");
            std::cout << "Deposited " << base << " 10" << std::endl;
            baseBalance = 10;
        }

        // use only 10% of available base for this product
        double totalAskAmount = baseBalance * 0.10;
        double askAmountPerOrder = totalAskAmount / 5.0;

        for (int i = 0; i < 5; ++i)
        {
            if (askAmountPerOrder <= 0)
                break;

            OrderBookEntry ask{
                bestAsk,
                askAmountPerOrder,
                currentTime,
                product,
                OrderBookType::ask
            };
            ask.username = user.getUsername();

            if (wallet.canFulfillOrder(ask))
            {
                orderBook.insertOrder(ask);
                transactionLogger.log(currentTime, ask.price, ask.amount, product, "ask");
            }
        }

        // --------------------
        // BIDS (buy base using quote)
        // --------------------
        double quoteBalance = wallet.getAmount(quote);
        if (quoteBalance == 0.0)
        {
            wallet.deposit(quote, 10);
            transactionLogger.log(currentTime, 0.0, 10, quote, "deposit");
            std::cout << "Deposited " << quote << " 10" << std::endl;
            quoteBalance = 10;
        }

        // use only 10% of available quote for this product
        double totalQuoteToSpend = quoteBalance * 0.10;
        double bidAmountPerOrder = (totalQuoteToSpend / 5.0) / bestBid;

        for (int i = 0; i < 5; ++i)
        {
            if (bidAmountPerOrder <= 0)
                break;

            OrderBookEntry bid{
                bestBid,
                bidAmountPerOrder,
                currentTime,
                product,
                OrderBookType::bid
            };
            bid.username = user.getUsername();

            if (wallet.canFulfillOrder(bid))
            {
                orderBook.insertOrder(bid);
                transactionLogger.log(currentTime, bid.price, bid.amount, product, "bid");
            }
        }
    }
}


void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = user.getUsername();;
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                transactionLogger.log(currentTime, obe.price, obe.amount, obe.product, "ask");
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = user.getUsername();

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                transactionLogger.log(currentTime, obe.price, obe.amount, obe.product, "bid");
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime, user);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == user.getUsername())
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
    wallet.saveToCSV();
}

void MerkelMain::modifyFunds()
{
    // Create a list of valid currencies
    std::vector<std::string> validCurrencies;
    for (const std::string& product : orderBook.getKnownProducts())
    {
        std::vector<std::string> currs = CSVReader::tokenise(product, '/');
        std::string base = currs[0];
        std::string quote = currs[1];

        // Add base currency if not already in the list
        if (std::find(validCurrencies.begin(), validCurrencies.end(), base) == validCurrencies.end())
            validCurrencies.push_back(base);

        // Add quote currency if not already in the list
        if (std::find(validCurrencies.begin(), validCurrencies.end(), quote) == validCurrencies.end())
            validCurrencies.push_back(quote);
    }

    // Get user input
    std::string input;
    std::cout << "Enter transaction (e.g. deposit BTC 5 or withdraw USDT 100): " << std::endl;
    std::getline(std::cin, input);

    std::stringstream ss(input);
    std::string action, currency;
    double amount;
    ss >> action >> currency >> amount;

    // Convert action to lowercase
    std::transform(action.begin(), action.end(), action.begin(), ::tolower);

    // Convert currency to upper
    std::transform(currency.begin(), currency.end(), currency.begin(), ::toupper);

    // Step 3: Validate currency
    if (std::find(validCurrencies.begin(), validCurrencies.end(), currency) == validCurrencies.end())
    {
        std::cout << "Invalid currency. Valid options are: ";
        for (const auto& c : validCurrencies) std::cout << c << " ";
        std::cout << std::endl;
        return;
    }

    // Step 4: Perform deposit or withdrawal
    if (action == "deposit")
    {
        if (wallet.deposit(currency, amount))
        {
            std::cout << "Deposit successful." << std::endl;
            transactionLogger.log(currentTime, 0.0, amount, currency, "deposit");
        }
        else
        {
            std::cout << "Deposit failed." << std::endl;
        }
    }
    else if (action == "withdraw")
    {
        if (wallet.withdraw(currency, amount))
        {
            std::cout << "Withdrawal successful." << std::endl;
            transactionLogger.log(currentTime, 0.0, amount, currency, "withdraw");
        }
        else
        {
            std::cout << "Insufficient balance or invalid amount." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid action. Use 'deposit' or 'withdraw'." << std::endl;
    }
}

void MerkelMain::printSummary()
{
    std::string choice;
    std::cout << "Choose option:\n";
    std::cout << "1: Summary statistics\n";
    std::cout << "2: View recent transactions\n";
    std::getline(std::cin, choice);

    if (choice == "1")
    {
        std::string input;
        std::cout << "Enter product (or ALL), start time, end time (comma separated):" << std::endl;
        std::cout << "Example: BTC/USDT,2020/06/01 10:00:00" << std::endl;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::string product, startTime;

        std::getline(ss, product, ',');
        std::getline(ss, startTime, ',');

        // List of known products
        std::vector<std::string> knownProducts = orderBook.getKnownProducts();
        std::transform(product.begin(), product.end(), product.begin(), ::toupper);

        // Validating input
        if (std::find(knownProducts.begin(), knownProducts.end(), product) != knownProducts.end() || product == "ALL") 
        {
            if (DateTime::isValidFormat(startTime))
            {
                if (product == "ALL")
                {
                    product = "";
                }

                transactionLogger.computeSummary(product, startTime);
            }
            else
            {
                std::cout << "Enter a valid start time." << std::endl;
            }
        } 
        else
        {
            std::cout << "Enter a valid product." << std::endl;
        }
    }
    else if (choice == "2")
    {
        std::string product;
        std::cout << "Enter product to filter (or ALL for last 5 transactions): ";
        std::getline(std::cin, product);

        std::transform(product.begin(), product.end(), product.begin(), ::toupper);

        // List of valid currencies
        std::vector<std::string> validCurrencies;
        for (const std::string& prod : orderBook.getKnownProducts())
        {
            std::vector<std::string> currs = CSVReader::tokenise(prod, '/');
            std::string base = currs[0];
            std::string quote = currs[1];

            if (std::find(validCurrencies.begin(), validCurrencies.end(), base) == validCurrencies.end())
                validCurrencies.push_back(base);

            if (std::find(validCurrencies.begin(), validCurrencies.end(), quote) == validCurrencies.end())
                validCurrencies.push_back(quote);
        }

        // List of known products
        std::vector<std::string> knownProducts = orderBook.getKnownProducts();

        if (product == "ALL")
        {
            product = "";
            transactionLogger.printRecentTransactions(5, product);
        }
        else
        {
            // Validate input
            bool isValidProduct = std::find(knownProducts.begin(), knownProducts.end(), product) != knownProducts.end();
            bool isValidCurrency = std::find(validCurrencies.begin(), validCurrencies.end(), product) != validCurrencies.end();

            if (isValidProduct || isValidCurrency) 
            {
                transactionLogger.printRecentTransactions(5, product);
            }
            else
            {
                std::cout << "Invalid product or currency. Please try again." << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Invalid option." << std::endl;
    }
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-10" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

bool MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0)
    {
        std::cout << "See you again " << user.getFullName() << "."<< std::endl;
        return false; // signal to exit loop
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        generateBidsAndAsks();
    }
    if (userOption == 4) 
    {
        enterAsk();
    }
    if (userOption == 5) 
    {
        enterBid();
    }
    if (userOption == 6) 
    {
        printWallet();
    }
    if (userOption == 7) 
    {
        ComputeCandlesticks::GetCandlesticks(currentTime);
    }   
    if (userOption == 8) 
    {
        modifyFunds();
    }       
    if (userOption == 9)
    {
        printSummary();
    }
    if (userOption == 10)
    {
        gotoNextTimeframe();
    }
    // for other options than the one listed
    else if (userOption > 10)
    {
        std::cout << "Invalid option choose 1-10 or 0 to exit. " << std::endl;
    }

    return true;
}
