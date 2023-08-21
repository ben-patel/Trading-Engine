#include "order.h"
#include <iostream>

namespace TradingEngine::Order {
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
       price { price }, quantity { quantity } {
            std::cout << "created order " << id << std::endl;
       }

    Order::~Order() {
        std::cout << "destroyed order " << id << std::endl;
    }
}