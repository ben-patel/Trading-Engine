#include "order.h"

namespace TradingEngine::Data::Order {
    Order::Order(
        uint64_t id,
        uint32_t symbolId,
        uint64_t userId,
        OrderType type,
        OrderSide side,
        OrderLifetime lifetime,
        int64_t price,
        uint32_t quantity
    ): id{ id }, symbolId{ symbolId }, userId { userId }, type{ type }, side{ side }, lifetime{ lifetime },
       price { price }, quantity { quantity } {}
}