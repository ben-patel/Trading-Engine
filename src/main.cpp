#include <iostream>
#include <cstdint>
#include "order.h"
#include "orderBook.h"

// toggle debug
#define DEBUG_MODE

/* Allocate memory on heap when 'new' function is called*/
void* operator new(size_t size) {
    #ifdef DEBUG_MODE
    std::cout << "    DEBUG: ALLOCATING " << size << " BYTES OF MEMORY" << std::endl;
    #endif
    return malloc(size);
}   

void printOrder(const TradingEngine::Data::Order::Order& o) {
    /* use c_string arrays to avoid mem allocation */
    const char* orderTypes[] = { "LIMIT", "MARKET" };
    const char* orderSides[] = { "BUY", "SELL" };
    const char* orderLifetimes[] = {        
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
    std::cout << "  QUANTITY: " << o.quantity << std::endl;
}

void printEntry(const TradingEngine::Data::LimitOrderBook::BookEntry& entry, bool orderPrint) {
    std::cout << "ENTRY: {\n";
    if (entry.next == nullptr) {
        std::cout << "  NEXT ENTRY: NULL\n";
    } else {
        std::cout << "  NEXT ENTRY: NOT NULL\n";
    }

    if (orderPrint) printOrder(entry.order);
    std::cout << "}" << std::endl;
}   

int main() {
    const TradingEngine::Data::Order::Order order {
        1, // id
        2, // ticker
        TradingEngine::Data::Order::OrderType::LIMIT, 
        TradingEngine::Data::Order::OrderSide::BUY,
        TradingEngine::Data::Order::OrderLifetime::GFD, 
        10, // price
        100 // size
    };

    const TradingEngine::Data::LimitOrderBook::BookEntry entry { order };

    #ifdef DEBUG_MODE
    printEntry(entry, true);
    std::cout << "\nRun \" export MallocStackLogging=1; leaks --atExit --list -- ./TE; unset MallocStackLogging; \" to debug memory leaks." << std::endl;
    #endif

    return 0;
}