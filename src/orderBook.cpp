#include "orderBook.h"
#include "order.h"

namespace TradingEngine::OrderBook {
    BookEntry::BookEntry(TradingEngine::Order::Order order): order{ order }, next { nullptr } {}
}