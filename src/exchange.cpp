#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "exchange.hpp"
#define PRINT_LOGS false // NOTE TAKES 5x LONGER WITH this ENABLED

namespace TradingEngine::Exchange {
    Instrument::Instrument(uint32_t symbolId, std::string_view symbol) {
        this->symbolId = symbolId;
        this->symbol = symbol;
        TradingEngine::LimitOrderBook::LimitOrderBook book {symbolId, PRINT_LOGS};
        this->orderBook = book;
    }

    Exchange& Exchange::getInstance() {
        static Exchange instance;
        return instance;
    }

    uint32_t Exchange::addInstrument(std::string_view symbol) {
        instruments[currSymbolId] = std::make_unique<Instrument>(currSymbolId, symbol);
        return currSymbolId++;
    }

    uint64_t Exchange::sendOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (instrumentThreads[symbolId] == nullptr) {
            // create new thread and send order in it
        } else {
            // add order to thread queue
        }
        return instruments[symbolId]->orderBook.addOrder(symbolId, userId, type, side, lifetime, price, quantity);

        // std::promise<uint64_t> promise;
        // std::future<uint64_t> future = promise.get_future();
        // if (instrumentThreads[symbolId] == nullptr) {
        //     instrumentThreads[symbolId] = std::make_unique<std::thread>([&promise, this, symbolId, userId, type, side, lifetime, price, quantity]() {
        //         uint64_t orderId = instruments[symbolId]->orderBook.addOrder(symbolId, userId, type, side, lifetime, price, quantity);
        //         promise.set_value(orderId);
        //     });

        //     instrumentThreads[symbolId]->join();
        //     instrumentThreads[symbolId] = nullptr;
        // } else {
        //     if (instrument)
        // }

        // return future.get();
    }
}