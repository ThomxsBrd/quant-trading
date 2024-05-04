#include "OrderBook.h"
#include "CSVReader.h"

#include <map>
#include <algorithm>

OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnowProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;
    for (OrderBookEntry& e:orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> order_sub;
    for(OrderBookEntry& e : orders)
    {
        if (e.orderType == type &&
            e.product == product && 
            e.timestamp == timestamp)
            {
                order_sub.push_back(e);
            }
    }
    return order_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }

    // case if the there is no timestamp greater than the price, start from the begining
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }

    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks in this timeframe
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    // bids = orderbook.bids in this timeframe
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    std::vector<OrderBookEntry> sales;
    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    for (OrderBookEntry &ask : asks)
    {
        for (OrderBookEntry &bid : bids)
        {
            if (bid.price >= ask.price) //we have a match
                {
                    OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                    if (bid.username == "simuser")
                    {
                        sale.username = "simuser";
                        sale.orderType = OrderBookType::bidsale;
                    }
                    if (ask.username == "simuser")
                    {
                        sale.username = "simuser";
                        sale.orderType = OrderBookType::asksale;
                     };

                    if (bid.amount == ask.amount) //  # bid completely clears ask

                    {
                        sale.amount = ask.amount;
                        sales.push_back(sale);
                        bid.amount = 0;
                        break;
                    }

                    if (bid.amount > ask.amount) // # ask is completely gone slice the bid
                    {
                        sale.amount = ask.amount;
                        sales.push_back(sale);
                        bid.amount = bid.amount - ask.amount;
                        break;
                    }

                    if (bid.amount < ask.amount &&
                        bid.amount > 0) // # bid is completely gone, slice the ask
                    {
                        sale.amount = bid.amount;
                        sales.push_back(sale);
                        ask.amount = ask.amount - bid.amount;
                        bid.amount = 0;
                        continue;
                    }
            }
        }
    }
    return sales;
}