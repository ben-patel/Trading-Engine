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

    float time = 0.0;
    for (size_t i = 0; i < 100; i++) {
        int ii = rand()%100;
        int j = rand()%100;
        int v1 = rand()%500 + 1;
        int v2 = rand()%500 + 1;

        TradingEngine::Order::OrderSide side = (v1 % 2) ? TradingEngine::Order::OrderSide::BUY : TradingEngine::Order::OrderSide::SELL;
        auto start = std::chrono::high_resolution_clock::now();
        uint64_t id = exchange.sendOrder(ii, 0, TradingEngine::Order::OrderType::LIMIT, side, TradingEngine::Order::OrderLifetime::GTC, v1, v2);
        if (v1 > 400) {
            exchange.cancelOrder(ii, id, 0);
        }

        auto end = std::chrono::high_resolution_clock::now();

        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    exchange.destroy();
    std::cout << time / 1000000 << std::endl;
    return 0;
}