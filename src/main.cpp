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

    if (orderPrint) printOrder(*entry.order);
    std::cout << "}" << std::endl;
}

int main() {
    TradingEngine::Order::OrderType type = TradingEngine::Order::OrderType::LIMIT;
    TradingEngine::Order::OrderSide side = TradingEngine::Order::OrderSide::BUY;
    TradingEngine::Order::OrderLifetime lifetime = TradingEngine::Order::OrderLifetime::GFD;

    TradingEngine::LimitOrderBook::LimitOrderBook book {};
    book.processLimit(2, 1, type, side, lifetime, 10, 100); // 1
    book.processLimit(2, 2, type, side, lifetime, 10, 50); // 2
    book.processLimit(1, 3, TradingEngine::Order::OrderType::LIMIT,
        TradingEngine::Order::OrderSide::SELL,
        TradingEngine::Order::OrderLifetime::GFD, 10, 10);

    #ifdef DEBUG_MODE
    printEntry(entry, true);
    #endif

    book.destroy();
    return 0;
}