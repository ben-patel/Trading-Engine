#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "exchange.hpp"

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
        if (instrumentThreads[symbolId] && instrumentThreads[symbolId]->joinable()) {
            instrumentThreads[symbolId]->join();
        }

        const std::shared_ptr<TradingEngine::Order::Order>& order = orderArena[orderId];
        if (order->userId != userId) {
            throw std::runtime_error("CANNOT CANCEL ORDER WITH DIFFERENT USER ID");
        }

        instruments[symbolId]->orderBook.cancelOrder(order);
    }

    uint64_t Exchange::sendOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        currOrderId.fetch_add(1);

        std::shared_ptr<TradingEngine::Order::Order> order = std::make_shared<TradingEngine::Order::Order>(
            currOrderId,
            symbolId,
            userId,
            type,
            side,
            lifetime,
            price,
            quantity,
            false
        );

        orderArena[currOrderId] = order;
        if (MULTITHREADING) {
            instruments[symbolId]->orderBook.addOrder(order);
            return currOrderId;
        }

        if (instrumentThreads[symbolId] != nullptr && instrumentThreads[symbolId]->joinable()) {
            instrumentThreads[symbolId]->join();
        }

        instrumentThreads[symbolId] = std::make_unique<std::thread>([this, symbolId, order]() {
            instruments[symbolId]->orderBook.addOrder(order);
        });

        return currOrderId;
    }

    void Exchange::destroy() {
        for (size_t i = 0; i < MAX_NUM_INSTRUMENTS; i++) {
            if (instrumentThreads[i] && instrumentThreads[i]->joinable()) instrumentThreads[i]->join();
        }
    }
}