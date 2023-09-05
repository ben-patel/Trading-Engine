#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "misc/util.hpp"

namespace TradingEngine::Trade {
    struct TradeResponse {
        uint32_t buyTraderId;
        uint32_t sellTraderId;
        uint32_t quantity;
        int32_t price;
        std::string_view symbol;
        TradingEngine::Util::ExchangeTime tradeTime;

        TradeResponse() = default;
        TradeResponse(
            uint32_t buyTraderId,
            uint32_t sellTraderId,
            uint32_t quantity,
            int32_t price,
            const std::string_view& symbol,
            const TradingEngine::Util::ExchangeTime& tradeTime
        );
    };

    class Trader {
    public:
        Trader(uint32_t userId, const std::string_view& institution, int32_t balance);

        /* Update balance after trader makes a trade */
        void makeTrade(bool buy, uint32_t otherId, uint32_t quantity, int32_t price, const std::string_view& symbol, const TradingEngine::Util::ExchangeTime& tradeTime);

        /* Getter */
        std::string_view getInstitution();

        /* Prints trade history */
        void printTrades();
    private:
        uint32_t id;
        int32_t balance;
        int32_t startingBalance;
        std::vector<TradeResponse> trades;
        std::string_view institution;
    };
};