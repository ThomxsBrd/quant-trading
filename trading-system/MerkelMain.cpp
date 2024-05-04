#include <iostream>
#include <vector>
#include <map>
#include <limits>

#include "OrderBookEntry.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "Wallet.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
        // 1 help
        std::cout << "1: Help" << std::endl;
        // 2 print exchange stats
        std::cout << "2: Print exchange stats" << std::endl;
        // 3 make an offer
        std::cout << "3: Make an Ask" << std::endl;
        // 4 make a bid
        std::cout << "4: Make a bid" << std::endl;
        // 5 print wallet
        std::cout << "5: Print wallet" << std::endl;
        // 6 continue
        std::cout << "6: Continue" << std::endl;

        // ask the user to selec an option
        std::cout << "=============================" << std::endl;

        std::cout << "Current time is: " << currentTime << std::endl;

        std::cout << "Select an option. Type in 1-6" << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - Your aim is to make money. Analyze the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{

    for (std::string const& p : orderBook.getKnowProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;

        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
    //std::cout << "OrderBook contains: " << orders.size() << " entries."<< std::endl;
    //unsigned int bids = 0;
    //unsigned int asks = 0;

    //for(OrderBookEntry& e : orders)
    //{
    //if (e.orderType == OrderBookType::ask)
    //{
    //asks++;
    //    }
    //if (e.orderType == OrderBookType::bid)
    //{
    //bids++;
    //    }
    //}
    //std::cout << "OrderBook asks : " << asks << " bids: " << bids << std::endl;
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);

            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds. " << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
    
    std::cout << "You typed:" << input << std::endl;
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);

            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds. " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "MerkelMain::enterBid Bad input " << std::endl;
        }
    };
}

void MerkelMain::printWallet()
{
    std::cout << "Please find the current state of your portfolio." << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::nextTimeFrame()
{
    std::cout << "Going to next time frame." << std::endl;

    for (std::string &p : orderBook.getKnowProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }

        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        /* code */
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
    }

    std::cout << "You chose " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    // map from ints to function pointers
    //std::map<int, void ()()> menu;
    //menu[1] = printHelp;
    //menu[2] = printMarketStats;
    //menu[3] = enterOffer;
    //menu[4] = enterBid;
    //menu[5] = printWallet;
    //menu[6] = nextTimeFrame;

    //menu[userOption]();

    if (userOption ==1){
        printHelp();
    };

    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        nextTimeFrame();
    }
}
