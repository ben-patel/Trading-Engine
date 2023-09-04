#include <cstdint>
#include "trader.hpp"

namespace TradingEngine::Trade {
    TradeResponse::TradeResponse(
        uint64_t buyTraderId,
        uint64_t sellTraderId,
        uint64_t quantity,
        int64_t price,
        uint64_t symbolId
    ): buyTraderId { buyTraderId }, sellTraderId { sellTraderId }, quantity { quantity }, price { price }, symbolId { symbolId } {}
}