#ifndef ORDERBOOK_H
#define ORDERBOOK_H

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
        static PricePoint pricePoints[MAX_PRICE];
        static uint64_t orderId;
        /* min and max repsective ask/bid prices */
        static int64_t minAsk;
        static int64_t maxBid;
        static uint32_t symbolId;

        /* Memory areana to avoid time-heavy heap operations */
        static BookEntry entriesArena[MAX_NUM_ORDERS];
        static BookEntry *arenaPtr;
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