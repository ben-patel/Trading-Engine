#include "util.hpp"

namespace TradingEngine::Util {
    ExchangeTime::ExchangeTime() {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct std::tm* timeInfo = std::localtime(&currentTime);
        unsigned int h = (unsigned int)timeInfo->tm_hour;
        unsigned int m = (unsigned int)timeInfo->tm_min;
        unsigned int s = (unsigned int)timeInfo->tm_sec;

        unsigned int ms = (unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch())
        .count() % 1000;

        unsigned int us = (unsigned int)std::chrono::duration_cast<std::chrono::microseconds>(
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