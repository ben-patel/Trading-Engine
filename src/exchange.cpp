#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "exchange.hpp"

namespace TradingEngine::Exchange {
    Instrument::Instrument(uint32_t symbolId, std::string_view symbol): symbolId { symbolId }, symbol { symbol } {
        TradingEngine::LimitOrderBook::LimitOrderBook book {symbolId, PRINT_LOGS};
        orderBook = book;
    }

    Exchange& Exchange::getInstance() {
        static Exchange instance;
        return instance;
    }

    uint32_t Exchange::addInstrument(std::string_view symbol) {
        instruments[currSymbolId] = std::make_unique<Instrument>(currSymbolId, symbol);
        return currSymbolId++;
    }

    uint32_t Exchange::addTrader(std::string_view institution, int32_t balance) {
        currTraderId.fetch_add(1);
        traderArena[currTraderId] = std::make_unique<TradingEngine::Trade::Trader>(currTraderId, institution, balance);
        return currTraderId;
    }

    void Exchange::cancelOrder(uint32_t symbolId, uint64_t orderId) {
        if (instrumentThreads[symbolId] && instrumentThreads[symbolId]->joinable()) {
            instrumentThreads[symbolId]->join();
        }

        const std::shared_ptr<TradingEngine::Order::Order>& order = orderArena[orderId];
        instruments[symbolId]->orderBook.cancelOrder(order);
    }

    uint64_t Exchange::sendOrder(uint32_t symbolId, uint64_t traderId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        currOrderId.fetch_add(1);
        std::shared_ptr<TradingEngine::Trade::Trader> trader = traderArena[traderId];

        std::shared_ptr<TradingEngine::Order::Order> order = std::make_shared<TradingEngine::Order::Order>(
            currOrderId,
            symbolId,
            trader,
            type,
            side,
            lifetime,
            price,
            quantity,
            false
        );

        orderArena[currOrderId] = order;
        if (!MULTITHREADING) {
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

    void Exchange::pr() {
        traderArena[currTraderId - 1]->printTrades();
    }
}