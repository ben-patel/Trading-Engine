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

    void Exchange::cancelOrder(uint32_t symbolId, uint64_t orderId, uint64_t userId) {
        instruments[symbolId]->orderBook.cancelOrder(userId, orderId);
    }

    void Exchange::sendOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        //instruments[symbolId]->orderBook.addOrder(symbolId, userId, type, side, lifetime, price, quantity);

        if (instrumentThreads[symbolId] != nullptr && instrumentThreads[symbolId]->joinable()) {
            instrumentThreads[symbolId]->join();
        }

        instrumentThreads[symbolId] = std::make_unique<std::thread>([this, symbolId, userId, type, side, lifetime, price, quantity]() {
            instruments[symbolId]->orderBook.addOrder(symbolId, userId, type, side, lifetime, price, quantity);
        });

    }

    void Exchange::destroy() {
        for (size_t i = 0; i < MAX_NUM_INSTRUMENTS; i++) {
            if (instrumentThreads[i] && instrumentThreads[i]->joinable()) instrumentThreads[i]->join();
        }
    }
}