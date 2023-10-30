#pragma once
#include <iostream>
#include <string>
#include "crow.h"

#include "../exchange/exchange.hpp"
#include "../misc/exchangeFactory.hpp"
#define SERVER_PORT 3000

namespace TradingEngine {
    class Server {
        TradingEngine::Exchange::Exchange &exchange;
    public:
        Server(std::string exchangeOption);
        void start();
        void stop();
    };
};