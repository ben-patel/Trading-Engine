#include <iostream>
#include "order.hpp"

namespace TradingEngine::Order {
    Order::Order(
        uint64_t id,
        uint32_t symbolId,
        std::shared_ptr<TradingEngine::Trade::Trader> trader,
        OrderType type,
        OrderSide side,
        OrderLifetime lifetime,
        int64_t price,
        uint32_t quantity,
        bool isActive
    ): id{ id }, symbolId{ symbolId }, trader { trader }, type{ type }, side{ side }, lifetime{ lifetime },
       price { price }, quantity { quantity }, isActive { isActive } {}
}