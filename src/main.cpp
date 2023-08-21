#include <iostream>
#include <cstdint>
#include "order.h"
#include "orderBook.h"

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

    book.destroy();
    return 0;
}