#include <iostream>
#include "order.hpp"

namespace TradingEngine::Order {
    Order::Order(
        uint32_t id,
        uint16_t symbolId,
        const std::shared_ptr<TradingEngine::Trade::Trader>& trader,
        const OrderType& type,
        const OrderSide& side,
        const OrderLifetime& lifetime,
        int32_t price,
        uint32_t quantity,
        bool isActive
    ): id{ id }, symbolId{ symbolId }, trader { trader }, type{ type }, side{ side }, lifetime{ lifetime },
       price { price }, quantity { quantity }, isActive { isActive } {}
}