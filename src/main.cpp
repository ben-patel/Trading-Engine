#include <iostream>
#include <cstdint>
#include <chrono>
#include <time.h>
#include <iomanip>
#include "orders/order.hpp"
#include "orders/orderBook.hpp"
#include "exchange.hpp"

int main(int argc, char * argv[]) {
    TradingEngine::Exchange::Exchange& exchange = TradingEngine::Exchange::Exchange::getInstance();
    const char* mostTradedStocks[] = {
        "AAPL",  // Apple Inc.
        "MSFT",  // Microsoft Corporation
        "AMZN",  // Amazon.com Inc.
        "GOOGL", // Alphabet Inc. (Google)
        "TSLA",  // Tesla Inc.
        "FB",    // Facebook Inc.
        "BRK.B", // Berkshire Hathaway Inc.
        "NVDA",  // NVIDIA Corporation
        "JPM",   // JPMorgan Chase & Co.
        "V",     // Visa Inc.
        "JNJ",   // Johnson & Johnson
        "PG",    // Procter & Gamble Co.
        "UNH",   // UnitedHealth Group Incorporated
        "MA",    // Mastercard Incorporated
        "HD",    // The Home Depot Inc.
        "PYPL",  // PayPal Holdings Inc.
        "BABA",  // Alibaba Group Holding Limited
        "DIS",   // The Walt Disney Company
        "VZ",    // Verizon Communications Inc.
        "BAC"    // Bank of America Corporation
    };

    for (int i = 0; i < 20; i++) {
        exchange.addInstrument(mostTradedStocks[i]);
    }

    float time = 0.0;
    for (size_t i = 0; i <= atoi(argv[1]); i++) {
        int ii = rand()%20;
        int j = rand()%20;
        int v1 = rand()%500 + 1;
        int v2 = rand()%500 + 1;

        TradingEngine::Order::OrderSide side = (v1 % 2) ? TradingEngine::Order::OrderSide::BUY : TradingEngine::Order::OrderSide::SELL;
        auto start = std::chrono::high_resolution_clock::now();
        exchange.sendOrder(ii, 0, TradingEngine::Order::OrderType::LIMIT, side, TradingEngine::Order::OrderLifetime::GTC, v1, v2);
        auto end = std::chrono::high_resolution_clock::now();

        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    std::cout << time / 1000000 << std::endl;
    return 0;
}