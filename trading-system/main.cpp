#include <iostream>
#include <string> 
#include <vector>

#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "Wallet.h"


int main()
{
    MerkelMain app{};
    app.init();
    //Wallet wallet;
    //wallet.insertCurrency("BTC", 10);
    //wallet.insertCurrency("USDT", 10000);
    //std::cout << "Wallet has BTC " << wallet.containsCurrency("BTC", 10) << std::endl;
    //std::cout << wallet.toString() << std::endl;

    //wallet.removeCurrency("USDT", 1000);
    //std::cout << wallet.toString() << std::endl;
}

// tokenizer end algorithm while (end != std::string::npos);