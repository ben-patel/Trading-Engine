#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"
#define MAX_PRICE 1000001

namespace TradingEngine::Data::LimitOrderBook {

    /* single order in limit order book */
    typedef struct BookEntry {
        TradingEngine::Order::Order order;
        BookEntry *next;

        BookEntry() = default;
        BookEntry(TradingEngine::Order::Order order);
        
    } BookEntry;

    /* single price point in limit order book */
    typedef struct PricePoint {
        BookEntry *listStart;
        BookEntry *listEnd;
    } PricePoint;

    /* Array of prices which represents entire book
       Static avoids heap based memory time buildup
    */
    static PricePoint pricePoints[MAX_PRICE];

}

#endif