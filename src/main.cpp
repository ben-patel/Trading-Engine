#include <iostream>
#include <cstdint>
#include "order.h"
#include "orderBook.h"

// toggle debug
// #define DEBUG_MODE

/* Allocate memory on heap when 'new' function is called*/
void* operator new(size_t size) {
    #ifdef DEBUG_MODE
    std::cout << "    DEBUG: ALLOCATING " << size << " BYTES OF MEMORY" << std::endl;
    #endif
    return malloc(size);
}

void printOrder(const TradingEngine::Order::Order& o) {
    constexpr std::string_view orderTypes[] = { "LIMIT", "MARKET" };
    constexpr std::string_view orderSides[] = { "BUY", "SELL" };
    constexpr std::string_view orderLifetimes[] = {
        "FOK",
        "IOC",
        "GFD",
        "GTD",
        "GTC"
    };

    std::cout << "  ORDER ID: " << o.id << '\n';
    std::cout << "  SYMBOL ID: " << o.symbolId << '\n';
    std::cout << "  USER ID: " << o.userId << '\n';
    std::cout << "  TYPE: " << orderTypes[(uint8_t)o.type] << '\n';
    std::cout << "  SIDE: " << orderSides[(uint8_t)o.side] << '\n';
    std::cout << "  LIFETIME: " << orderLifetimes[(uint8_t)o.lifetime] << '\n';
    std::cout << "  PRICE: " << o.price << '\n';
    std::cout << "  QUANTITY: " << o.quantity << std::endl;
}

void printEntry(const TradingEngine::LimitOrderBook::BookEntry& entry, bool orderPrint) {
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
    const TradingEngine::Order::Order order {
        1, // id
        2, // ticker
        0, // user id
        TradingEngine::Order::OrderType::LIMIT,
        TradingEngine::Order::OrderSide::BUY,
        TradingEngine::Order::OrderLifetime::GFD,
        10, // price
        100 // size
    };

    const TradingEngine::LimitOrderBook::BookEntry entry { order };

    #ifdef DEBUG_MODE
    printEntry(entry, true);
    #endif

    return 0;
}