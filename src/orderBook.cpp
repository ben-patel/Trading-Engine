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
        pricePoints = *(new std::vector<std::shared_ptr<PricePoint>>(MAX_PRICE));
        orderArena = *(new std::vector<std::shared_ptr<TradingEngine::Order::Order>>(MAX_NUM_ORDERS));

        for (size_t i = 0; i < MAX_PRICE; i++) {
            pricePoints[i] = std::make_shared<PricePoint>();
        }
    }

    void LimitOrderBook::executeTrade(TradingEngine::Order::Order& order1, TradingEngine::Order::Order& order2, uint64_t quantity) {
        std::cout << "executing " << quantity << " lots between orders " << order1.id << " and " << order2.id << '\n';
    }

    void LimitOrderBook::insertOrder(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        std::shared_ptr<PricePoint> pricePoint = pricePoints[order->price];

        std::shared_ptr<BookEntry> entry = std::make_shared<BookEntry>(order);
        if (pricePoint->listStart == nullptr) {
            pricePoint->listStart = entry;
        } else {
            pricePoint->listEnd->next = entry;
        }

        pricePoint->listEnd = entry;
    }

    uint64_t LimitOrderBook::processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->price >= minAsk) {
            do {
                std::shared_ptr<PricePoint> pricePoint = pricePoints[minAsk];
                std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

                while(currEntry != nullptr) {
                    const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;
                    if (currOrder->quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder->quantity < order->quantity) {
                        executeTrade(*currOrder, *order, currOrder->quantity);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(*order, *currOrder, order->quantity);
                        currEntry = currEntry->next;
                        /* TODO: may want to free here? */
                        pricePoint->listStart = currEntry;
                        return currOrderId;
                    }
                }

            pricePoint->listStart = nullptr;
            minAsk++;
            } while (order->price >= minAsk);
        }

        insertOrder(order);
        if (maxBid < order->price) maxBid = order->price;

        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order) {
        if (order->price <= maxBid) {
            do {
                std::shared_ptr<PricePoint> pricePoint = pricePoints[maxBid];
                std::shared_ptr<BookEntry> currEntry = pricePoint->listStart;

                while(currEntry != nullptr) {
                    const std::shared_ptr<TradingEngine::Order::Order> currOrder = currEntry->order;
                    if (currOrder->quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder->quantity < order->quantity) {
                        executeTrade(*currOrder, *order, currOrder->quantity);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(*order, *currOrder, order->quantity);
                        currEntry = currEntry->next;
                        /* TODO: may want to free here? */
                        pricePoint->listStart = currEntry;
                        return currOrderId;
                    }
                }

            pricePoint->listStart = nullptr;
            maxBid--;
            } while (order->price <= maxBid);
        }

        insertOrder(order);
        if (minAsk < order->price) minAsk = order->price;

        return currOrderId;
    }

    /* returns order id */
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

    void LimitOrderBook::destroy() {
    }
}