#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "orders/orderBook.hpp"

namespace TradingEngine::Exchange {
    constexpr uint16_t MAX_NUM_INSTRUMENTS { 1000 };

    typedef struct Instrument {
        uint32_t symbolId;
        std::string_view symbol;
        TradingEngine::LimitOrderBook::LimitOrderBook orderBook;

        Instrument(uint32_t symbolId, std::string_view symbol);
    } Instrument;

    class Exchange {
    public:
        static Exchange& getInstance();
        uint32_t addInstrument(std::string_view symbol);
        void cancelOrder(uint32_t symbolId, uint64_t orderId, uint64_t userId);
        void destroy();
        void sendOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity);

    private:
        Exchange() : currSymbolId { 0 }, numThreadsRunning { 0 } {}
        Exchange(const Exchange&) = delete;
        Exchange& operator=(const Exchange&) = delete;
        uint32_t currSymbolId;
        std::atomic<int> numThreadsRunning;
        std::unique_ptr<Instrument> instruments[MAX_NUM_INSTRUMENTS];
        std::unique_ptr<std::thread> instrumentThreads[MAX_NUM_INSTRUMENTS];
    };
}