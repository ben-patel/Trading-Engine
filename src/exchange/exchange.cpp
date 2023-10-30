#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "exchange.hpp"

extern std::string symbols[];

namespace TradingEngine::Exchange {
    Instrument::Instrument(uint16_t symbolId, const std::string_view& symbol ): symbolId { symbolId }, symbol { symbol } {
        TradingEngine::LimitOrderBook::LimitOrderBook book {symbolId, PRINT_LOGS};
        orderBook = book;
    }

    Exchange& Exchange::getInstance() {
        static Exchange instance;
        return instance;
    }

    uint32_t Exchange::getInstrumentId(const std::string& symbol) {
        if (symbol == "" || instrumentIds.find(symbol) == instrumentIds.end()) {
            throw std::runtime_error("GET INSTRUMENT: Invalid symbol");
        }

        return instrumentIds[symbol];
    }

    uint32_t Exchange::addInstrument(const std::string& symbol) {
        if (symbol == "") {
            throw std::runtime_error("ADD INSTRUMENT: Invalid symbol");
        }

        symbols[currSymbolId] = symbol;
        instruments[currSymbolId] = std::make_unique<Instrument>(currSymbolId, symbol);
        instrumentIds[symbol] = currSymbolId;
        return currSymbolId++;
    }

    uint32_t Exchange::addTrader(const std::string_view& institution, int32_t balance) {
        if (institution == "") {
            throw std::runtime_error("ADD TRADER: Institution cannot be empty string");
        }

        currTraderId.fetch_add(1);
        traderArena[currTraderId] = std::make_unique<TradingEngine::Trade::Trader>(currTraderId, institution, balance);
        return currTraderId;
    }

    uint32_t Exchange::modifyOrder(uint16_t symbolId, uint32_t orderId, uint32_t traderId, const TradingEngine::Order::OrderType& type, const TradingEngine::Order::OrderSide& side,
        const TradingEngine::Order::OrderLifetime& lifetime, int32_t price, uint32_t quantity) {
        if (symbolId < 0 || price < 0 || quantity == 0 || type != TradingEngine::Order::OrderType::LIMIT || instruments[symbolId] == nullptr || orderArena[orderId] == nullptr) {
            throw std::runtime_error("MODIFY ORDER: Invalid input");
        }

        cancelOrder(symbolId, orderId);

        if (PRINT_LOGS) {
            std::cout << TradingEngine::Util::ExchangeTime().getString() << ": " << orderId << " MODIFY ORDER" << std::endl;
        }
        return sendOrder(symbolId, traderId, type, side, lifetime, price, quantity);
    }

    void Exchange::cancelOrder(uint16_t symbolId, uint32_t orderId) {
        if (orderId < 0 || symbolId < 0 || instruments[symbolId] == nullptr || orderArena[orderId] == nullptr) {
            throw std::runtime_error("CANCEL ORDER: Invalid order id");
        }

        const std::shared_ptr<TradingEngine::Order::Order>& order = orderArena[orderId];
        if (order == nullptr) {
            throw std::runtime_error("CANCEL ORDER: Invalid order id");
        } else if (symbolId != order->symbolId || instruments[symbolId] == nullptr) {
            throw std::runtime_error("CANCEL ORDER: Invalid symbol id");
        }

        if (instrumentThreads[symbolId] && instrumentThreads[symbolId]->joinable()) {
            instrumentThreads[symbolId]->join();
        }

        instruments[symbolId]->orderBook.cancelOrder(order);
    }

    uint32_t Exchange::sendOrder(uint16_t symbolId, uint32_t traderId, const TradingEngine::Order::OrderType& type, const TradingEngine::Order::OrderSide& side,
        const TradingEngine::Order::OrderLifetime& lifetime, int32_t price, uint32_t quantity) {
        if (symbolId < 0 || instruments[symbolId] == nullptr || traderArena[traderId] == nullptr || price < 0 || quantity == 0 ||
            traderArena[traderId] == nullptr || type != TradingEngine::Order::OrderType::LIMIT) {
            throw std::runtime_error("SEND ORDER: Invalid input");
        }

        currOrderId.fetch_add(1);
        std::shared_ptr<TradingEngine::Trade::Trader> trader = traderArena[traderId];

        std::string_view symbol = instruments[symbolId]->symbol;
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

    void Exchange::printTrades(uint32_t traderId) {
        if (traderId < 0 || traderArena[traderId] == nullptr) {
            throw std::runtime_error("PRINT TRADES: Invalid trader id");
        }

        traderArena[traderId]->printTrades();
    }
}