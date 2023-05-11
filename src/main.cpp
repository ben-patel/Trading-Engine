#include <iostream>
#include "order.h"

int main(void) {
    TradingEngine::Order::Order order {
        1, // id
        2, // stock id
        TradingEngine::Order::OrderType::LIMIT, 
        TradingEngine::Order::OrderSide::BUY,
        TradingEngine::Order::OrderLifetime::GFD, 
        10, // price
        100 // vol
    };

    return 0;
}