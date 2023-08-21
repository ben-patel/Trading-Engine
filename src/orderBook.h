#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <memory>
#include <vector>
#include "order.h"

/* Limit order book inspired by QUANT CUP1 winner Voyager */
namespace TradingEngine::LimitOrderBook {
    constexpr uint8_t TICKS_PER_UNIT { 100 };
    constexpr int64_t MAX_PRICE { TICKS_PER_UNIT * 1000 };
    constexpr uint64_t INVALID_ORDER_ID { 1000002 };
    constexpr uint8_t EMPTY { 0 };
    constexpr size_t MAX_NUM_ORDERS { 1000001 };

    /* single order in limit order book */
    typedef struct BookEntry {
        const std::shared_ptr<TradingEngine::Order::Order> order;
        std::shared_ptr<BookEntry> next;

        BookEntry() = default;
        BookEntry(const std::shared_ptr<TradingEngine::Order::Order>& order) : order(order) {
            next = nullptr;
        };
    } BookEntry;

    /* single price point in limit order book */
    typedef struct PricePoint {
        std::shared_ptr<BookEntry> listStart;
        std::shared_ptr<BookEntry> listEnd;

        PricePoint();
    } PricePoint;

    class LimitOrderBook {
    public:
        std::vector<std::shared_ptr<PricePoint>> pricePoints;
        std::vector<std::shared_ptr<TradingEngine::Order::Order>> orderArena;
        uint64_t currOrderId;
        int64_t minAsk;
        int64_t maxBid;
        uint32_t symbolId;

        LimitOrderBook();
        void insertOrder(const std::shared_ptr<TradingEngine::Order::Order>& order);
        /* TODO: execture trades */
        void executeTrade(TradingEngine::Order::Order& order1, TradingEngine::Order::Order& order2, uint64_t quantity);
        void cancel(uint64_t orderId);
        void destroy();
        uint64_t processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order);
        uint64_t processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order);
        uint64_t processLimit(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
            TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity);
    };

}

#endif