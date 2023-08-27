#pragma once
#include <cstdint>

namespace TradingEngine::Order {
    enum class OrderType {
        LIMIT,
        MARKET
    };

    enum class OrderSide {
        BUY,
        SELL
    };

    enum class OrderLifetime {
        FOK,
        IOC,
        GFD,
        GTD,
        GTC
    };

    typedef struct Order {
        OrderSide side;
        OrderType type;
        OrderLifetime lifetime;
        uint64_t id;
        uint64_t symbolId;
        uint64_t userId;
        uint64_t quantity;
        int64_t price;
        bool isActive;

        Order() = default;
        Order(
            uint64_t id,
            uint32_t symbolId,
            uint64_t userId,
            OrderType type,
            OrderSide side,
            OrderLifetime lifetime,
            int64_t price,
            uint32_t quantity,
            bool isActive
        );
    } Order;
}