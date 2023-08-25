#include <iostream>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include "orders/order.hpp"
#include "orders/orderBook.hpp"
#include "exchange.hpp"

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

void print(std::multiset<int64_t> myMultiset) {
    for (std::multiset<int64_t>::iterator it = myMultiset.begin(); it != myMultiset.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Enter number of orders" << std::endl;
        return 1;
    }

    TradingEngine::Exchange::Exchange& exchange = TradingEngine::Exchange::Exchange::getInstance();
    std::cout << exchange.addInstrument("GME") << std::endl;
    std::cout << exchange.addInstrument("GME") << std::endl;

    TradingEngine::Order::OrderType type = TradingEngine::Order::OrderType::LIMIT;
    TradingEngine::Order::OrderSide side = TradingEngine::Order::OrderSide::BUY;
    TradingEngine::Order::OrderLifetime lifetime = TradingEngine::Order::OrderLifetime::GFD;

    TradingEngine::LimitOrderBook::LimitOrderBook book {1, false};
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < atoi(argv[1]); i++) {
        uint32_t q = (uint32_t)(rand() % 998) + 1;
        int64_t price = (uint32_t)(rand() % 998) + 1;
        uint64_t id;

        if (i%2 == 0) {
            // std::cout << "book.processLimit(1, 3, TradingEngine::Order::OrderType::LIMIT,\n";
            // std::cout << "TradingEngine::Order::OrderSide::BUY,\n";
            // std::cout << "TradingEngine::Order::OrderLifetime::GFD, " << price << ", " << q <<");\n";

            id = book.processLimit(1, 3, TradingEngine::Order::OrderType::LIMIT,
                TradingEngine::Order::OrderSide::BUY,
                TradingEngine::Order::OrderLifetime::GFD, price, q);
        } else {
            // std::cout << "book.processLimit(1, 3, TradingEngine::Order::OrderType::LIMIT,\n";
            // std::cout << "TradingEngine::Order::OrderSide::SELL,\n";
            // std::cout << "TradingEngine::Order::OrderLifetime::GFD, " << price << ", " << q << ");\n";
            id = book.processLimit(1, 3, TradingEngine::Order::OrderType::LIMIT,
                TradingEngine::Order::OrderSide::SELL,
                TradingEngine::Order::OrderLifetime::GFD, price, q);
        }

        if (q > 800) {
           // book.cancelOrder(3, id);
            //std::cout << "book.cancelOrder(3, " << id << ")\n";
        }

        //std::cout << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << std::fixed << std::setprecision(6) << duration << std::endl;

    book.destroy();
    return 0;
}