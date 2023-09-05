#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <iomanip>
#include <sstream>
#include "orderBook.hpp"

namespace TradingEngine::LimitOrderBook {
    std::mutex lock;
    PricePoint::PricePoint() {
        listStart = nullptr;
        listEnd = nullptr;
    }

    LimitOrderBook::LimitOrderBook(uint32_t symbolId, bool printLogs): printLogs { printLogs }, symbolId { symbolId }, minAsk { MAX_PRICE }, maxBid { -1 },
        pricePoints { std::vector<std::shared_ptr<PricePoint>>(MAX_PRICE) }, bidPrices { std::multiset<int64_t>() }, askPrices { std::multiset<int64_t>() } {
        for (size_t i = 0; i < MAX_PRICE; i++) {
            pricePoints[i] = std::make_shared<PricePoint>();
        }
    }

    void LimitOrderBook::addOrder(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->type == TradingEngine::Order::OrderType::LIMIT) {
            processLimit(order);
        } else {
            throw std::runtime_error("ERROR: Only accepting limit orders right now.");
        }
    }

    void LimitOrderBook::executeTrade(const std::shared_ptr<TradingEngine::Order::Order>& order1, const std::shared_ptr<TradingEngine::Order::Order>& order2, uint64_t quantity, int64_t price) {
        if (order1->side == order2->side) {
            throw std::runtime_error("ERROR: TRADE ORDERS HAVE SAME SIDE");
        }

        uint64_t buyer = (order1->side == TradingEngine::Order::OrderSide::BUY) ? order1->id : order2->id;
        uint64_t seller = (order1->side == TradingEngine::Order::OrderSide::SELL) ? order1->id : order2->id;
        TradingEngine::Util::ExchangeTime tradeTime;

        if (printLogs) {
            lock.lock();
            std::cout << tradeTime.getString() << ": " << order1->symbolId << " TRADE " << quantity << " @ " << price << " [" << order1->trader->getInstitution() << ", " << order2->trader->getInstitution() << "]" << std::endl;
            std::cout << std::flush;
            lock.unlock();
        }

        order1->trader->makeTrade((order1->id == buyer), order2->id, quantity, price, order1->symbolId, tradeTime);
        order2->trader->makeTrade((order2->id == buyer), order1->id, quantity, price, order2->symbolId, tradeTime);
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

    void LimitOrderBook::processLimit(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->side == TradingEngine::Order::OrderSide::BUY) {
            processLimitBuy(order);
        } else if (order->side == TradingEngine::Order::OrderSide::SELL) {
            processLimitSell(order);
        }

    }

    uint64_t LimitOrderBook::cancelOrder(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->side == TradingEngine::Order::OrderSide::BUY && order->isActive) {
            bidPrices.erase(bidPrices.find(order->price));
        } else if (order->isActive) {
            askPrices.erase(askPrices.find(order->price));
        }

        order->quantity = 0;
        order->isActive = false;

        if (printLogs) {
            lock.lock();
            TradingEngine::Util::ExchangeTime cancelTime;
            std::cout << cancelTime.getString() << ": " << order->id << " CANCEL" << std::endl;
            lock.unlock();
        }
        return order->id;
    }

    void LimitOrderBook::destroy() {
    }
}