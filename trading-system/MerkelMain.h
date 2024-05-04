#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the simulation */
        void init();
    private:
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void nextTimeFrame();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;

        OrderBook orderBook{"data.csv"};

        Wallet wallet;
};