#pragma once

#include <memory>
#include <vector>
#include "order.h"

/* Limit order book inspired by QUANT CUP1 winner Voyager */
namespace TradingEngine::LimitOrderBook {
    constexpr uint8_t TICKS_PER_UNIT { 100 };
    constexpr int64_t MAX_PRICE { TICKS_PER_UNIT * 1000 };
    constexpr int8_t MIN_PRICE { -1 };
    constexpr uint64_t INVALID_ORDER_ID { 1000002 };
    constexpr uint8_t EMPTY { 0 };
    constexpr size_t MAX_NUM_ORDERS { 1000001 };

    /* Order book entry */
    typedef struct BookEntry {
        const std::shared_ptr<TradingEngine::Order::Order> order;
        std::shared_ptr<BookEntry> next;

        BookEntry() = default;
        BookEntry(const std::shared_ptr<TradingEngine::Order::Order>& order) : order(order) {
            next = nullptr;
        };
    } BookEntry;

    /* Price in the order book, contains list of entries at that price */
    typedef struct PricePoint {
        std::shared_ptr<BookEntry> listStart;
        std::shared_ptr<BookEntry> listEnd;

        PricePoint();
    } PricePoint;

    class LimitOrderBook {
    public:
        /* Allocates memory for book */
        LimitOrderBook();
        /* Add order with given info to book */
        uint64_t addOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity);
        // TODO: exectute trade method
        void executeTrade(TradingEngine::Order::Order& order1, TradingEngine::Order::Order& order2, uint64_t quantity, int64_t price);
        /* Inserts given order into the appropriate price point list */
        void insertOrder(const std::shared_ptr<TradingEngine::Order::Order>& order);
        /* Inserts limit order to book */
        uint64_t processLimit(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
            TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity);
        /* Free book memory */
        void destroy();
        /* Cancel order with given id, returns order id */
        uint64_t cancelOrder(uint64_t userId, uint64_t orderId);

    private:
        std::vector<std::shared_ptr<PricePoint>> pricePoints;
        std::vector<std::shared_ptr<TradingEngine::Order::Order>> orderArena;
        uint64_t currOrderId;
        int64_t minAsk;
        int64_t maxBid;
        uint32_t symbolId;

        void updateMinAsk(const uint64_t orderId);
        void updateMaxBuy(const uint64_t orderId);
        uint64_t processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order);
        uint64_t processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order);
    };

}