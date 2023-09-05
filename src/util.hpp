#include <cstdint>
#include <string>

namespace TradingEngine::Util {
    struct ExchangeTime {
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
        uint32_t millis;
        uint32_t micros;

        ExchangeTime();
        std::string getString();
    };
}