#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "util.hpp"

namespace TradingEngine::Trade {
    struct TradeResponse {
        uint64_t buyTraderId;
        uint64_t sellTraderId;
        uint64_t quantity;
        int64_t price;
        uint64_t symbolId;
        TradingEngine::Util::ExchangeTime tradeTime;

        TradeResponse() = default;
        TradeResponse(
            uint64_t buyTraderId,
            uint64_t sellTraderId,
            uint64_t quantity,
            int64_t price,
            uint64_t symbolId,
            TradingEngine::Util::ExchangeTime tradeTime
        );
    };

    class Trader {
    public:
        Trader(uint64_t userId, std::string_view institution, int32_t balance);

        /* Update balance after trader makes a trade */
        void makeTrade(bool buy, uint64_t otherId, uint64_t quantity, int64_t price, uint64_t symbolId, TradingEngine::Util::ExchangeTime tradeTime);

        /* Getter */
        std::string_view getInstitution();

        /* Prints trade history */
        void printTrades();
    private:
        uint64_t id;
        int32_t balance;
        int32_t startingBalance;
        std::vector<TradeResponse> trades;
        std::string_view institution;
    };
};