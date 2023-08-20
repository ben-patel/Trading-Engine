#include <iostream>
#include "orderBook.h"
#include "order.h"

namespace TradingEngine::LimitOrderBook {
    BookEntry::BookEntry(TradingEngine::Order::Order order): order{ order }, next { nullptr } {}

    LimitOrderBook::LimitOrderBook() {
        currOrderId = 0;
        minAsk = MAX_PRICE;
        maxBid = -1;
        pricePoints = std::vector<PricePoint>(MAX_PRICE);
        orderArena = std::vector<TradingEngine::Order::Order*>(MAX_NUM_ORDERS);
    }

    void LimitOrderBook::executeTrade(TradingEngine::Order::Order& order1, TradingEngine::Order::Order& order2, uint64_t quantity) {
        std::cout << "executing " << quantity << " lots between orders " << order1.id << " and " << order2.id << '\n';
    }

    void LimitOrderBook::insertOrder(TradingEngine::Order::Order& order) {
        int64_t price = order.price;
        PricePoint pricePoint = pricePoints[price];

        BookEntry entry { order };
        BookEntry *entryAddress = &entry;
        entry.next = nullptr;
        if (pricePoint.listStart == nullptr) {
            pricePoint.listStart = entryAddress;
        } else {
            pricePoint.listEnd->next = entryAddress;
        }

        pricePoint.listEnd = entryAddress;
    }

    uint64_t LimitOrderBook::processLimitBuy(TradingEngine::Order::Order& order) {
        PricePoint *pricePoint;

        if (order.price >= minAsk) {
            pricePoint = &pricePoints[minAsk];
            BookEntry *currEntry = pricePoint->listStart;

            do {
                while(currEntry != nullptr) {
                    TradingEngine::Order::Order currOrder = currEntry->order;
                    if (currOrder.quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder.quantity < order.quantity) {
                        executeTrade(currOrder, order, currOrder.quantity);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(order, currOrder, order.quantity);
                        currEntry = currEntry->next;
                        /* TODO: may want to free here? */
                        pricePoint->listStart = currEntry;
                        return currOrderId;
                    }
                }

            pricePoint->listStart = nullptr;
            pricePoint++;
            minAsk++;
            } while (order.price >= minAsk);
        }

        insertOrder(order);
        if (minAsk < order.price) minAsk = order.price;

        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimitSell(TradingEngine::Order::Order& order) {
        PricePoint *pricePoint;

        if (order.price <= maxBid) {
            pricePoint = &pricePoints[maxBid];
            BookEntry *currEntry = pricePoint->listStart;

            do {
                while(currEntry != nullptr) {
                    TradingEngine::Order::Order currOrder = currEntry->order;
                    if (currOrder.quantity == EMPTY) {
                        currEntry = currEntry->next;
                        continue;
                    }

                    if (currOrder.quantity < order.quantity) {
                        executeTrade(currOrder, order, currOrder.quantity);
                        currEntry = currEntry->next;
                    } else {
                        executeTrade(order, currOrder, order.quantity);
                        currEntry = currEntry->next;
                        /* TODO: may want to free here? */
                        pricePoint->listStart = currEntry;
                        return currOrderId;
                    }
                }

            pricePoint->listStart = nullptr;
            pricePoint--;
            maxBid--;
            } while (order.price <= maxBid);
        }

        insertOrder(order);
        if (maxBid < order.price) maxBid = order.price;

        return currOrderId;
    }

    /* returns order id */
    uint64_t LimitOrderBook::processLimit(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {

        TradingEngine::Order::Order order {
            ++currOrderId,
            symbolId,
            userId,
            type,
            side,
            lifetime,
            price,
            quantity
        };

        orderArena[currOrderId] = &order;
        if (order.side == TradingEngine::Order::OrderSide::BUY) {
            return processLimitBuy(order);
        } else if (order.side == TradingEngine::Order::OrderSide::SELL) {
            return processLimitSell(order);
        }

        return INVALID_ORDER_ID;
    }
}