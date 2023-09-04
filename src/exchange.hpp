#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "orders/orderBook.hpp"
#define NYSE_NUM_LISTINGS 2500

namespace TradingEngine::Exchange {
    constexpr uint16_t MAX_NUM_INSTRUMENTS { NYSE_NUM_LISTINGS };

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

        /* Sends an order to the exchange with the given details */
        uint64_t sendOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity, bool multihreading);

        /* Cancels given order */
        void cancelOrder(uint32_t symbolId, uint64_t orderId, uint64_t userId);

        /* Free exchange memory after usage */
        void destroy();

    private:
        Exchange() : currSymbolId { 0 }, currOrderId { 0 } {}
        Exchange(const Exchange&) = delete;
        Exchange& operator=(const Exchange&) = delete;
        uint32_t currSymbolId;
        std::atomic<uint64_t> currOrderId;
        std::unique_ptr<Instrument> instruments[MAX_NUM_INSTRUMENTS];
        std::unique_ptr<std::thread> instrumentThreads[MAX_NUM_INSTRUMENTS];
    };
}