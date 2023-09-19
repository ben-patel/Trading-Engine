#include <iostream>
#include <cstdint>
#include <chrono>
#include <time.h>
#include <iomanip>
#include "orders/order.hpp"
#include "orders/orderBook.hpp"
#include "exchange/exchange.hpp"
#include "misc/exchangeFactory.hpp"
using namespace TradingEngine;

int main() {
    std::cout << "Creating exchange..." << std::endl;
    TradingEngine::Exchange::Exchange& exchange = *(TradingEngine::Util::ExchangeFactory::getExchange("taiwanIndiaSymbols"));
    uint32_t a = (uint32_t)exchange.addTrader("ben", 1000000);
    uint32_t b = (uint32_t)exchange.addTrader("joe", -100000);
    uint32_t c = (uint32_t)exchange.addTrader("deez", -100000);
    uint32_t d = (uint32_t)exchange.addTrader("bob", -10000000);
    uint32_t traders[] = {a,b,c,d};
    float time = 0.0;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "Finished" << std::endl;
    for (size_t i = 0; i < 100; i++) {
        uint16_t ii = (uint16_t)rand()%100;
        uint32_t j = (uint32_t)rand()%100;
        uint32_t v1 = (uint32_t)rand()%500 + 1;
        int32_t v3 = (int32_t)rand()%500 + 1;
        int32_t v4 = (int32_t)rand()%500 + 1;
        uint32_t v2 = (uint32_t)rand()%500 + 1;


        Order::OrderSide side = (v1 % 2) ? Order::OrderSide::BUY : Order::OrderSide::SELL;
        auto start = std::chrono::high_resolution_clock::now();
        int trader = rand()%4;
        uint32_t id = exchange.sendOrder(ii, traders[trader], Order::OrderType::LIMIT, side, Order::OrderLifetime::GTC, v4, v2);
        if (v1 > 480) {
            exchange.cancelOrder(ii, id);
        } else if (v1 < 20) {
            exchange.modifyOrder(ii, id, traders[trader], Order::OrderType::LIMIT, side, Order::OrderLifetime::GTC, v3, v1);
        }

        auto end = std::chrono::high_resolution_clock::now();

        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    exchange.printTrades(a);
    std::cout << "Freeing memory..." << std::endl;
    exchange.destroy();
    std::cout << time / 1000000 << std::endl;
    return 0;
}