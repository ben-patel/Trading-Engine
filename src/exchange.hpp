#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "orders/orderBook.hpp"
#include "trader.hpp"

/* Takes 5-10x longer when either are enabled */
#define PRINT_LOGS true
#define MULTITHREADING false

namespace TradingEngine::Exchange {
    constexpr size_t MAX_NUM_INSTRUMENTS { 2500 };
    constexpr size_t MAX_NUM_ORDERS { 1000001 };
    constexpr size_t MAX_NUM_TRADERS { 100000 };

    typedef struct Instrument {
        uint32_t symbolId;
        std::string_view symbol;
        TradingEngine::LimitOrderBook::LimitOrderBook orderBook;

        Instrument(uint32_t symbolId, std::string_view symbol);
    } Instrument;

    class Exchange {
    public:
        /* Gettter for what is practically singleton pattern for exchange */
        static Exchange& getInstance();

        /* Adds a new book to exchange with new symbol */
        uint32_t addInstrument(std::string_view symbol);

        /* Adds a new trader to this exchange */
        uint32_t addTrader(std::string_view institution, int32_t balance);

        /* Sends an order to the exchange with the given details */
        uint64_t sendOrder(uint32_t symbolId, uint64_t traderId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity);

        /* Cancels given order */
        void cancelOrder(uint32_t symbolId, uint64_t orderId);

        /* Free exchange memory after usage */
        void destroy();

        void pr();

    private:
        Exchange() : currSymbolId { 0 }, currOrderId { 0 }, currTraderId { 0 } {}
        Exchange(const Exchange&) = delete;
        Exchange& operator=(const Exchange&) = delete;

        uint32_t currSymbolId;
        std::atomic<uint64_t> currOrderId;
        std::atomic<uint64_t> currTraderId;
        std::unique_ptr<Instrument> instruments[MAX_NUM_INSTRUMENTS];
        std::unique_ptr<std::thread> instrumentThreads[MAX_NUM_INSTRUMENTS];
        std::shared_ptr<TradingEngine::Order::Order> orderArena[MAX_NUM_ORDERS];
        std::shared_ptr<TradingEngine::Trade::Trader> traderArena[MAX_NUM_TRADERS];
    };
}