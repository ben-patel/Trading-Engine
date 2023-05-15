#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include "order.h"
#include "orderBook.h"

// toggle debug
#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG
#endif

#ifdef DEBUG
void printOrder(TradingEngine::Order::Order o) {
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

void printEntry(TradingEngine::OrderBook::BookEntry entry, bool orderPrint) {
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
    TradingEngine::Order::Order order {
        1, // id
        2, // ticker
        TradingEngine::Order::OrderType::LIMIT, 
        TradingEngine::Order::OrderSide::BUY,
        TradingEngine::Order::OrderLifetime::GFD, 
        10, // price
        100 // size
    };

    TradingEngine::OrderBook::BookEntry entry { order };
    
    #ifdef DEBUG
    printEntry(entry, true);
    #endif
    return 0;
}