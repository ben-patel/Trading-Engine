#include <iostream>
#include "orderBook.h"
#include "order.h"

namespace TradingEngine::LimitOrderBook {
    BookEntry::BookEntry(TradingEngine::Order::Order order): order{ order }, next { nullptr } {}

}