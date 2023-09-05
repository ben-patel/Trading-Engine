#include <cstdint>
#include <string>

namespace TradingEngine::Util {
    struct ExchangeTime {
        unsigned int hours;
        unsigned int minutes;
        unsigned int seconds;
        unsigned int millis;
        unsigned int micros;

        ExchangeTime();
        std::string getString();
    };
}