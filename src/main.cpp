#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include "order.h"
#include "orderBook.h"

// toggle debug
//#define DEBUG_MODE

#ifdef DEBUG_MODE
void printOrder(TradingEngine::Data::Order::Order o) {
    std::vector<std::string> orderTypes{ "LIMIT", "MARKET" };
    std::vector<std::string> orderSides{ "BUY", "SELL" };
    std::vector<std::string> orderLifetimes{        
        "FOK",
        "IOC",
        "GFD",
        "GTD",
        "GTC" 
    };

    std::cout << "  ORDER ID: " << o.id << '\n';
    std::cout << "  SYMBOL ID: " << o.symbolId << '\n';
    std::cout << "  TYPE: " << orderTypes[(uint8_t)o.type] << '\n';
    std::cout << "  SIDE: " << orderSides[(uint8_t)o.side] << '\n';
    std::cout << "  LIFETIME: " << orderLifetimes[(uint8_t)o.lifetime] << '\n';
    std::cout << "  PRICE: " << o.price << '\n';
    std::cout << "  QUANTITY: " << o.quantity << '\n';
}

void printEntry(TradingEngine::Data::LimitOrderBook::BookEntry entry, bool orderPrint) {
    std::cout << "ENTRY: {\n";
    if (entry.next == nullptr) {
        std::cout << "  NEXT ENTRY: NULL\n";
    } else {
        std::cout << "  NEXT ENTRY: NOT NULL\n";
    }

    if (orderPrint) printOrder(entry.order);
    std::cout << "}" << std::endl;
}   
#endif

int main(void) {
    TradingEngine::Data::Order::Order order {
        1, // id
        2, // ticker
        TradingEngine::Data::Order::OrderType::LIMIT, 
        TradingEngine::Data::Order::OrderSide::BUY,
        TradingEngine::Data::Order::OrderLifetime::GFD, 
        10, // price
        100 // size
    };

    TradingEngine::Data::LimitOrderBook::BookEntry entry { order };
    
    #ifdef DEBUG_MODE
    printEntry(entry, true);
    #endif
    return 0;
}