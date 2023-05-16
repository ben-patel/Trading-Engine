#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <memory>
#include "order.h"
#define MAX_PRICE 1000001
#define MAX_NUM_ORDERS 1000001

/* Limit order book inspired by QUANT CUP1 winner Voyager */
namespace TradingEngine::Data::LimitOrderBook {

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
        PricePoint pricePoints[MAX_PRICE];
        uint64_t orderId;
        /* min and max repsective ask/bid prices */
        int64_t minAsk;
        int64_t maxBid;
        uint32_t symbolId;

        /* Memory areana to avoid time-heavy heap operations */
        BookEntry entriesArena[MAX_NUM_ORDERS];
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