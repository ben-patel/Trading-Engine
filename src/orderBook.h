#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"

namespace TradingEngine::OrderBook {
    typedef struct BookEntry {
        TradingEngine::Order::Order order;
        BookEntry *next;

        BookEntry() = default;
        BookEntry(TradingEngine::Order::Order order);
        
    } BookEntry;
}

#endif