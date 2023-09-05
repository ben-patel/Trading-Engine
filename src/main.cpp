#include <iostream>
#include <cstdint>
#include <chrono>
#include <time.h>
#include <iomanip>
#include "orders/order.hpp"
#include "orders/orderBook.hpp"
#include "exchange.hpp"

int main() {
    TradingEngine::Exchange::Exchange& exchange = TradingEngine::Exchange::Exchange::getInstance();
    const char* mostTradedStocks[] = {
        "AAPL",
        "MSFT",
        "AMZN",
        "GOOGL",
        "TSLA",
        "FB",
        "BRK.B",
        "NVDA",
        "JPM",
        "V",
        "JNJ",
        "PG",
        "UNH",
        "MA",
        "HD",
        "PYPL",
        "BABA",
        "DIS",
        "VZ",
        "BAC"
    };

    for (int i = 0; i < 100; i++) {
        exchange.addInstrument(mostTradedStocks[i%20]);
    }

    uint64_t a = exchange.addTrader("optiver", 1000000);
    uint64_t b = exchange.addTrader("joe", -100000);
    uint64_t c = exchange.addTrader("deez", -100000);
    uint64_t d = exchange.addTrader("bob", -10000000);
    uint64_t traders[] = {a,b,c,d};
    float time = 0.0;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < 200; i++) {
        uint32_t ii = (uint32_t)rand()%100;
        uint32_t j = (uint32_t)rand()%100;
        int64_t v1 = (int64_t)rand()%500 + 1;
        uint32_t v2 = (uint32_t)rand()%500 + 1;

        TradingEngine::Order::OrderSide side = (v1 % 2) ? TradingEngine::Order::OrderSide::BUY : TradingEngine::Order::OrderSide::SELL;
        auto start = std::chrono::high_resolution_clock::now();
        size_t trader = rand()%4;
        uint64_t id = exchange.sendOrder(ii, traders[trader], TradingEngine::Order::OrderType::LIMIT, side, TradingEngine::Order::OrderLifetime::GTC, v1, v2);
        if (v1 > 450) {
            exchange.cancelOrder(ii, id);
        } else if (v1 < 20) {
            exchange.modifyOrder(ii, id, traders[trader], TradingEngine::Order::OrderType::LIMIT, side, TradingEngine::Order::OrderLifetime::GTC, v2, v1);
        }

        auto end = std::chrono::high_resolution_clock::now();

        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    exchange.printTrades(a);
    exchange.destroy();
    std::cout << time / 1000000 << std::endl;
    return 0;
}