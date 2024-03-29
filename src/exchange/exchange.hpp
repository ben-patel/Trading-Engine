#pragma once
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>
#include <atomic>
#include "../orders/orderBook.hpp"
#include "trader.hpp"

/* Takes 5-10x longer when either are enabled */
#define PRINT_LOGS true
#define MULTITHREADING false

namespace TradingEngine::Exchange {
    constexpr size_t MAX_NUM_INSTRUMENTS { 2500 };
    constexpr size_t MAX_NUM_ORDERS { 100001 }; // 100k
    constexpr size_t MAX_NUM_TRADERS { 10000 };

    typedef struct Instrument {
        uint16_t symbolId;
        std::string_view symbol;
        TradingEngine::LimitOrderBook::LimitOrderBook orderBook;

        Instrument(uint16_t symbolId, const std::string_view& symbol);
    } Instrument;

    class Exchange {
    public:
        /* Gettter for what is practically singleton pattern for exchange */
        static Exchange& getInstance();

        /* Returns id for given symbol */
        uint32_t getInstrumentId(const std::string& symbol);

        /* Adds a new book to exchange with new symbol */
        uint32_t addInstrument(const std::string& symbol);

        /* Adds a new trader to this exchange */
        uint32_t addTrader(const std::string_view& institution, int32_t balance);

        /* Sends an order to the exchange with the given details */
        uint32_t sendOrder(uint16_t symbolId, uint32_t traderId, const TradingEngine::Order::OrderType& type, const TradingEngine::Order::OrderSide& side,
            const TradingEngine::Order::OrderLifetime& lifetime, int32_t price, uint32_t quantity);

        /* Modify given order, note: loses time priority after modification */
        uint32_t modifyOrder(uint16_t symbolId, uint32_t orderId, uint32_t traderId, const TradingEngine::Order::OrderType& type, const TradingEngine::Order::OrderSide& side,
            const TradingEngine::Order::OrderLifetime& lifetime, int32_t price, uint32_t quantity);

        /* Cancels given order */
        void cancelOrder(uint16_t symbolId, uint32_t orderId);

        /* Free exchange memory after usage */
        void destroy();

        void printTrades(uint32_t traderId);

    private:
        Exchange() : currSymbolId { 0 }, currOrderId { 0 }, currTraderId { 0 } {}
        Exchange(const Exchange&) = delete;
        Exchange& operator=(const Exchange&) = delete;

        uint32_t currSymbolId;
        std::atomic<uint32_t> currOrderId;
        std::atomic<uint32_t> currTraderId;
        std::unique_ptr<Instrument> instruments[MAX_NUM_INSTRUMENTS];
        std::unordered_map<std::string, uint32_t> instrumentIds;
        std::unique_ptr<std::thread> instrumentThreads[MAX_NUM_INSTRUMENTS];
        std::shared_ptr<TradingEngine::Order::Order> orderArena[MAX_NUM_ORDERS];
        std::shared_ptr<TradingEngine::Trade::Trader> traderArena[MAX_NUM_TRADERS];
    };
}