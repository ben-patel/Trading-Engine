#include "order.h"

namespace TradingEngine::Order {
    Order::Order(uint64_t id,
              uint32_t symbolId,
              OrderType type,
              OrderSide side,
              OrderLifetime lifetime,
              int64_t price,
              uint32_t quantity
    ): id{ id }, symbolId{ symbolId }, type{ type }, side{ side }, lifetime{ lifetime },
       price { price }, quantity { quantity } {}
}