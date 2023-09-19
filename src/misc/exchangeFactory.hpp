#include <string>
#include "../exchange/exchange.hpp"

namespace TradingEngine::Util {
    class ExchangeFactory {
    public:
        static TradingEngine::Exchange::Exchange* getExchange(std::string exchangeFile);
    };
}