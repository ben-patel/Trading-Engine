#include <cstdint>
#include <iostream>
#include "trader.hpp"

namespace TradingEngine::Trade {
    TradeResponse::TradeResponse(
        uint64_t buyTraderId,
        uint64_t sellTraderId,
        uint64_t quantity,
        int64_t price,
        uint64_t symbolId,
        TradingEngine::Util::ExchangeTime tradeTime
    ): buyTraderId { buyTraderId }, sellTraderId { sellTraderId }, quantity { quantity }, price { price }, symbolId { symbolId }, tradeTime { tradeTime } {}

    Trader::Trader(uint64_t id, std::string_view institution, int32_t balance): id { id }, institution { institution }, balance { balance }, startingBalance { balance } {}

    void Trader::makeTrade(bool buy, uint64_t otherId, uint64_t quantity, int64_t price, uint64_t symbolId, TradingEngine::Util::ExchangeTime tradeTime) {
        uint64_t cost = (uint64_t)price * quantity;
        balance -= (buy ? cost : -cost);

        uint64_t buyTraderId = buy ? id : otherId;
        uint64_t sellTraderId = buy ? otherId : id;
        trades.push_back(TradeResponse(buyTraderId, sellTraderId, quantity, price, symbolId, tradeTime));
    }

    void Trader::printTrades() {
        std::cout << "Trades for " << institution << ":" << std::endl;
        std::cout << std::endl;
        for (TradeResponse trade : trades) {
            std::cout << "Trade at " << trade.tradeTime.getString() << ":\n";
            std::cout << ((trade.buyTraderId == id) ? "Buy" : "Sell") << "\n";
            std::cout << "Quantity: " << trade.quantity << "\n";
            std::cout << "Price: " << trade.price << "\n";
            std::cout << "Symbol: " << trade.symbolId << "\n";
            std::cout << std::endl;
        }

        int32_t profit = balance - startingBalance;
        std::cout << "Ending Balance: " << balance << " (" << ((profit >= 0) ? "+" : "" ) << profit << ")" << std::endl;
    }

    std::string_view Trader::getInstitution() {
        return institution;
    }
}