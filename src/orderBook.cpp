#include <iostream>
#include "orderBook.h"
#include "order.h"

namespace TradingEngine::LimitOrderBook {
    PricePoint::PricePoint() {
        listStart = nullptr;
        listEnd = nullptr;
    }

    LimitOrderBook::LimitOrderBook() {
        currOrderId = 0;
        minAsk = MAX_PRICE;
        maxBid = -1;
        pricePoints = std::vector<std::shared_ptr<PricePoint>>(MAX_PRICE);
        orderArena = std::vector<std::shared_ptr<TradingEngine::Order::Order>>(MAX_NUM_ORDERS);

        for (size_t i = 0; i < MAX_PRICE; i++) {
            pricePoints[i] = std::make_shared<PricePoint>();
        }
    }

    uint64_t LimitOrderBook::addOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (type == TradingEngine::Order::OrderType::LIMIT) {
            return processLimit(symbolId, userId, type, side, lifetime, price, quantity);
        } else {
            /* Only supports limit orders RN. */
            return INVALID_ORDER_ID;
        }
    }

    void LimitOrderBook::executeTrade(TradingEngine::Order::Order& order1, TradingEngine::Order::Order& order2, uint64_t quantity, int64_t price) {
        uint64_t buyer = (order1.side == TradingEngine::Order::OrderSide::BUY) ? order1.id : order2.id;
        uint64_t seller = (order1.side == TradingEngine::Order::OrderSide::SELL) ? order1.id : order2.id;
        std::cout << "ORDER ID " << buyer << " BUY " << quantity << " @ " << price << '\n';
        std::cout << "ORDER ID " << seller << " SELL " << quantity << " @ " << price << '\n';
        std::cout << std::endl;

        order1.quantity -= quantity;
        order2.quantity -= quantity;
    }

    void LimitOrderBook::insertOrder(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        std::shared_ptr<PricePoint> pricePoint = pricePoints[(size_t)order->price];

        std::shared_ptr<BookEntry> entry = std::make_shared<BookEntry>(order);
        if (pricePoint->listStart == nullptr) {
            pricePoint->listStart = entry;
        } else {
            pricePoint->listEnd->next = entry;
        }

        pricePoint->listEnd = entry;
    }

    void LimitOrderBook::updateMinAsk(const uint64_t orderId) {
        bool foundPrice = false;
        for (size_t i = 0; i <= orderId; i++) {
            std::shared_ptr<TradingEngine::Order::Order> order = orderArena[i];
            if (order && order->side == TradingEngine::Order::OrderSide::SELL && order->quantity > EMPTY) {
                foundPrice = true;
                minAsk = std::min(minAsk, order->price);
            }
        }

        if (!foundPrice) {
            minAsk = MAX_PRICE;
        }
    }

    void  LimitOrderBook::updateMaxBuy(const uint64_t orderId) {
        bool foundPrice = false;
        for (size_t i = 0; i <= orderId; i++) {
            std::shared_ptr<TradingEngine::Order::Order> order = orderArena[i];
            if (order && order->side == TradingEngine::Order::OrderSide::BUY && order->quantity > EMPTY) {
                foundPrice = true;
                maxBid = std::max(maxBid, order->price);
            }
        }

        if (!foundPrice) {
            maxBid = MIN_PRICE;
        }
    }

    uint64_t LimitOrderBook::processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->price >= minAsk) {
            do {
                std::shared_ptr<PricePoint> pricePoint = pricePoints[(size_t)minAsk];
                std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

                while(currEntry != nullptr) {
                    const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;
                    if (currOrder->quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder->quantity < order->quantity) {
                        executeTrade(*currOrder, *order, currOrder->quantity, currOrder->price);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(*currOrder, *order, order->quantity, currOrder->price);
                        pricePoint->listStart = currEntry;
                        updateMaxBuy(order->id);
                        return currOrderId;
                    }
                }

                pricePoint->listStart = nullptr;
                updateMinAsk(order->id);
            } while (order->price >= minAsk);
        }

        insertOrder(order);
        updateMaxBuy(order->id);
        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->price <= maxBid) {
            do {
                std::shared_ptr<PricePoint> pricePoint = pricePoints[(size_t)maxBid];
                std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

                while(currEntry != nullptr) {
                    const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;
                    if (currOrder->quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder->quantity < order->quantity) {
                        executeTrade(*currOrder, *order, currOrder->quantity, currOrder->price);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(*currOrder, *order, order->quantity, currOrder->price);
                        pricePoint->listStart = currEntry;
                        updateMinAsk(order->id);
                        return currOrderId;
                    }
                }

                pricePoint->listStart = nullptr;
                updateMaxBuy(order->id);
            } while (order->price <= maxBid);
        }

        insertOrder(order);
        updateMinAsk(order->id);
        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimit(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {

        currOrderId++;
        std::shared_ptr<TradingEngine::Order::Order> order = std::make_shared<TradingEngine::Order::Order>(
            currOrderId,
            symbolId,
            userId,
            type,
            side,
            lifetime,
            price,
            quantity
        );

        orderArena[currOrderId] = order;
        if (order->side == TradingEngine::Order::OrderSide::BUY) {
            return processLimitBuy(order);
        } else if (order->side == TradingEngine::Order::OrderSide::SELL) {
            return processLimitSell(order);
        }

        return INVALID_ORDER_ID;
    }

    uint64_t LimitOrderBook::cancelOrder(uint64_t userId, uint64_t orderId) {
        std::shared_ptr<TradingEngine::Order::Order> order = orderArena[orderId];
        if (order->userId == userId) {
            order->quantity = 0;
            return order->id;
        } else {
            return INVALID_ORDER_ID;
        }
    }

    void LimitOrderBook::destroy() {
    }
}