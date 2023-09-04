#pragma once
#include <memory>
#include <vector>
#include <set>
#include "order.hpp"

/* Can easily increase the below constants, but I'm running this on my laptop, not exactly
   the scale a real stock exchange requires. */

/* Prices are represented as whole integers, but in cents. e.g. 100 = $1.00, 2506 = $25.06 */
namespace TradingEngine::LimitOrderBook {
    constexpr uint8_t TICKS_PER_UNIT { 100 };
    constexpr int64_t MAX_PRICE { TICKS_PER_UNIT * 1000 };
    constexpr int8_t MIN_PRICE { -1 };
    constexpr uint64_t INVALID_ORDER_ID { 1000002 };
    constexpr uint8_t EMPTY { 0 };

    /* Order book entry */
    typedef struct BookEntry {
        const std::shared_ptr<TradingEngine::Order::Order> order;
        std::shared_ptr<BookEntry> next;

        BookEntry() = default;
        BookEntry(const std::shared_ptr<TradingEngine::Order::Order>& order) : order(order) {
            next = nullptr;
        };
    } BookEntry;

    /* Price in the order book, contains list of entries at that price */
    typedef struct PricePoint {
        std::shared_ptr<BookEntry> listStart;
        std::shared_ptr<BookEntry> listEnd;

        PricePoint();
    } PricePoint;

    class LimitOrderBook {
    public:
        LimitOrderBook() = default;
        /* Allocates memory for book */
        LimitOrderBook(uint32_t symbolId, bool printLogs);

        /* Add order to book */
        void addOrder(const std::shared_ptr<TradingEngine::Order::Order>& order);

        void executeTrade(const std::shared_ptr<TradingEngine::Order::Order>& order1, const std::shared_ptr<TradingEngine::Order::Order>& order2, uint64_t quantity, int64_t price);

        /* Inserts given order into the appropriate price point list */
        void insertOrder(std::shared_ptr<PricePoint>& pricePoint, const std::shared_ptr<TradingEngine::Order::Order>& order);

        /* Free book memory */
        void destroy();

        /* Cancel order with given id, returns order id */
        uint64_t cancelOrder(uint64_t userId, uint64_t orderId);

    private:
        std::vector<std::shared_ptr<PricePoint>> pricePoints;
        std::multiset<int64_t> bidPrices;
        std::multiset<int64_t> askPrices;
        int64_t minAsk;
        int64_t maxBid;
        uint32_t symbolId;
        bool printLogs;

        void updateMinAsk();
        void updateMaxBuy();
        void processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order);
        void processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order);
        void processLimit(const std::shared_ptr<TradingEngine::Order::Order>& order);
    };

}