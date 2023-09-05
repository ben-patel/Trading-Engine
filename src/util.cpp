#include "util.hpp"

namespace TradingEngine::Util {
    ExchangeTime::ExchangeTime() {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct std::tm* timeInfo = std::localtime(&currentTime);
        uint64_t h = (uint64_t)timeInfo->tm_hour;
        uint64_t m = (uint64_t)timeInfo->tm_min;
        uint64_t s = (uint64_t)timeInfo->tm_sec;

        uint64_t ms = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch())
        .count() % 1000;

        uint64_t us = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch())
        .count() % 1000000;

        hours = h;
        minutes = m;
        seconds = s;
        millis = ms;
        micros = us;
    }

    std::string ExchangeTime::getString() {
        std::string h = (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
        std::string m = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
        std::string s = (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);

        std::string str = h + ":" + m + ":"  + std::to_string(seconds) + ":"  + std::to_string(millis) + ":"  + std::to_string(micros);
        return str;
    }
}