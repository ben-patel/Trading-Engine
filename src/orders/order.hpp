#pragma once
#include <cstdint>
#include "../trader.hpp"

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

    struct Order {
        OrderSide side;
        OrderType type;
        OrderLifetime lifetime;
        uint32_t id;
        uint16_t symbolId;
        uint32_t quantity;
        int32_t price;
        bool isActive;
        std::shared_ptr<TradingEngine::Trade::Trader> trader;

        Order() = default;
        Order(
            uint32_t id,
            uint16_t symbolId,
            const std::shared_ptr<TradingEngine::Trade::Trader>& trader,
            const OrderType& type,
            const OrderSide& side,
            const OrderLifetime& lifetime,
            int32_t price,
            uint32_t quantity,
            bool isActive
        );
    };
}