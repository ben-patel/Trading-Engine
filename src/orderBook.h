#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <memory>
#include "order.h"

/* Limit order book inspired by QUANT CUP1 winner Voyager */
namespace TradingEngine::Data::LimitOrderBook {
    constexpr int64_t max_price { 1000001 };
    constexpr size_t max_num_orders { 1000001 };

    /* single order in limit order book */
    typedef struct BookEntry {
        TradingEngine::Data::Order::Order order;
        BookEntry *next;

        BookEntry() = default;
        BookEntry(TradingEngine::Data::Order::Order order);
        
    } BookEntry;

    /* single price point in limit order book */
    typedef struct PricePoint {
        BookEntry *listStart;
        BookEntry *listEnd;
    } PricePoint;

    class LimitOrderBook {
    public:
        /* Array of prices which represents entire book */
        PricePoint pricePoints[max_price];
        uint64_t orderId;
        /* min and max repsective ask/bid prices */
        int64_t minAsk;
        int64_t maxBid;
        uint32_t symbolId;

        /* Memory areana to avoid time-heavy heap operations */
        BookEntry entriesArena[max_num_orders];
        std::unique_ptr<BookEntry> arenaPtr;
        #define ALLOCATE_ENTRY(id)

        LimitOrderBook();
        void destroy();
        void insertOrder(PricePoint *price, BookEntry *entry);
        /* TODO: Figure out params */
        void executeTrade();
        uint64_t processLimit(TradingEngine::Data::Order::Order order);
        void cancel(uint64_t orderId);
        /* TODO: Add modification */
    };

}

#endif