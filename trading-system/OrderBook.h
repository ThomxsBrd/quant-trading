#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** construct, reading a csv data file*/
        OrderBook(std::string filename);
    /** return vector of all known products in the dataset */
        std::vector<std::string> getKnowProducts();
    /** return vector of Orders accorfing to the sen filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                                std::string product,
                                                std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        /** returns the earliest time in the orderbook, assuming the times are ordered*/
        std::string getEarliestTime();

        /** return the next time after the sent time in the orderbook
        * If there is not next timestamp, wraps around to the start
        */
        std::string getNextTime(std::string timestamp);

        void insertOrder(OrderBookEntry &order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    private:
        std::vector<OrderBookEntry> orders;

        
};