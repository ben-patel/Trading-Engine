#include <iostream>
#include "orderBook.h"
#include "order.h"

namespace TradingEngine::Data::LimitOrderBook {
    BookEntry::BookEntry(TradingEngine::Data::Order::Order order): order{ order }, next { nullptr } {}

    /* TODO: Implement OrderBook class */
}