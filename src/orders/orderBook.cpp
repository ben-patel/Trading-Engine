#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <iomanip>
#include <sstream>
#include "orderBook.hpp"
#include "order.hpp"

namespace TradingEngine::LimitOrderBook {
    std::mutex lock;
    PricePoint::PricePoint() {
        listStart = nullptr;
        listEnd = nullptr;
    }

    LimitOrderBook::LimitOrderBook(uint32_t symbolId, bool printLogs) {
        minAsk = MAX_PRICE;
        maxBid = -1;
        pricePoints = std::vector<std::shared_ptr<PricePoint>>(MAX_PRICE);
        orderArena = std::vector<std::shared_ptr<TradingEngine::Order::Order>>(MAX_NUM_ORDERS);
        bidPrices = std::multiset<int64_t>();
        askPrices = std::multiset<int64_t>();
        this->printLogs = printLogs;
        this->symbolId = symbolId;

        for (size_t i = 0; i < MAX_PRICE; i++) {
            pricePoints[i] = std::make_shared<PricePoint>();
        }
    }

    void LimitOrderBook::addOrder(uint32_t symbolId, uint64_t orderId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (type == TradingEngine::Order::OrderType::LIMIT) {
            processLimit(symbolId, orderId, userId, type, side, lifetime, price, quantity);
        } else {
            throw std::runtime_error("ERROR: Only accepting limit orders right now.");
        }
    }

    std::string getTime() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

        struct tm timeinfo;
        localtime_r(&now_c, &timeinfo);

        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%H:%M:%S")
            << ":" << std::setw(3) << std::setfill('0') << millis.count()
            << ":" << std::setw(6) << std::setfill('0') << micros.count();
        return oss.str();
    }

    void LimitOrderBook::executeTrade(const std::shared_ptr<TradingEngine::Order::Order>& order1, const std::shared_ptr<TradingEngine::Order::Order>& order2, uint64_t quantity, int64_t price) {
        uint64_t buyer = (order1->side == TradingEngine::Order::OrderSide::BUY) ? order1->id : order2->id;
        uint64_t seller = (order1->side == TradingEngine::Order::OrderSide::SELL) ? order1->id : order2->id;

        if (printLogs) {
            lock.lock();
            std::cout << getTime() << ": " << order1->symbolId << " TRADE " << quantity << " @ " << price << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(25));
            std::cout << std::flush;
            lock.unlock();
        }

        if (order1->side == order2->side) {
            throw std::runtime_error("ERROR: TRADE ORDERS HAVE SAME SIDE");
        }

        order1->quantity -= quantity;
        order2->quantity -= quantity;

        if (order1->quantity < 0 || order2->quantity < 0) {
            throw std::runtime_error("ERROR: NEGATIVE QUANTITY REMAINING");
        }

        if (order1->quantity == EMPTY) {
            order1->isActive = false;
            if (order1->id == seller) {
                askPrices.erase(askPrices.find(order1->price));
            } else {
                bidPrices.erase(bidPrices.find(order1->price));
            }
        }
    }

    void LimitOrderBook::insertOrder(std::shared_ptr<PricePoint>& pricePoint, const std::shared_ptr<TradingEngine::Order::Order>& order) {
        std::shared_ptr<BookEntry> entry = std::make_shared<BookEntry>(order);

        if (pricePoint->listStart == nullptr) {
            pricePoint->listStart = entry;
        } else {
            pricePoint->listEnd->next = entry;
        }

        pricePoint->listEnd = entry;
    }

    void LimitOrderBook::updateMinAsk() {
        minAsk = !askPrices.empty() ? *(askPrices.begin()) : MAX_PRICE;
    }

    void  LimitOrderBook::updateMaxBuy() {
        maxBid = !bidPrices.empty() ? *(bidPrices.rbegin()) : MIN_PRICE;
    }

    void LimitOrderBook::processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        updateMinAsk();

        while (order->price >= minAsk) {
            std::shared_ptr<PricePoint> pricePoint = pricePoints[(size_t)minAsk];
            std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

            while(currEntry != nullptr) {
                const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;
                if (currOrder->quantity == EMPTY) {
                    currEntry = currEntry->next;
                    continue;
                }

                if (currOrder->quantity < order->quantity) {
                    executeTrade(currOrder, order, currOrder->quantity, currOrder->price);
                    currEntry = currEntry->next;
                } else {
                    executeTrade(currOrder, order, order->quantity, currOrder->price);
                    pricePoint->listStart = currEntry;
                    updateMaxBuy();

                    return;
                }
            }

            pricePoint->listStart = nullptr;
            updateMinAsk();
        }

        order->isActive = true;
        bidPrices.insert(order->price);
        insertOrder(pricePoints[(size_t)order->price], order);
        updateMaxBuy();

        return;
    }

    void LimitOrderBook::processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        updateMaxBuy();

        while (order->price <= maxBid) {
            std::shared_ptr<PricePoint> pricePoint = pricePoints[(size_t)maxBid];
            std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

            while(currEntry != nullptr) {
                const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;

                if (currOrder->quantity == EMPTY) {
                    currEntry = currEntry->next;
                    continue;
                }

                if (currOrder->quantity < order->quantity) {
                    executeTrade(currOrder, order, currOrder->quantity, currOrder->price);
                    currEntry = currEntry->next;
                } else {
                    executeTrade(currOrder, order, order->quantity, currOrder->price);
                    pricePoint->listStart = currEntry;
                    updateMinAsk();

                    return;
                }
            }

            pricePoint->listStart = nullptr;
            updateMaxBuy();
        }

        order->isActive = true;
        askPrices.insert(order->price);
        insertOrder(pricePoints[(size_t)order->price], order);
        updateMinAsk();
        return;
    }

    void LimitOrderBook::processLimit(uint32_t symbolId, uint64_t orderId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (quantity <= 0) {
            throw std::runtime_error("ERROR: INVALID ORDER QUANTITY");
        }

        std::shared_ptr<TradingEngine::Order::Order> order = std::make_shared<TradingEngine::Order::Order>(
            orderId,
            symbolId,
            userId,
            type,
            side,
            lifetime,
            price,
            quantity,
            false
        );

        orderArena[orderId] = order;
        if (order->side == TradingEngine::Order::OrderSide::BUY) {
            processLimitBuy(order);
        } else if (order->side == TradingEngine::Order::OrderSide::SELL) {
            processLimitSell(order);
        }

    }

    uint64_t LimitOrderBook::cancelOrder(uint64_t userId, uint64_t orderId) {
        std::shared_ptr<TradingEngine::Order::Order> order = orderArena[orderId];

        if (order->userId != userId) {
             return INVALID_ORDER_ID;
        }

        if (order->side == TradingEngine::Order::OrderSide::BUY && order->isActive) {
            bidPrices.erase(bidPrices.find(order->price));
        } else if (order->isActive) {
            askPrices.erase(askPrices.find(order->price));
        }

        order->quantity = 0;
        order->isActive = false;

        if (printLogs) {
            lock.lock();
            std::cout << getTime() << ": " << orderId << " CANCEL" << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(25));
            lock.unlock();
        }
        return order->id;
    }

    void LimitOrderBook::destroy() {
    }
}