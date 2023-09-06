#include <string>
#include "../exchange.hpp"

namespace TradingEngine::Util {
    class ExchangeFactory {
    public:
        static TradingEngine::Exchange::Exchange* getExchange(std::string exchangeFile);
    };
}