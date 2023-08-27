#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <iomanip>
#include <sstream>
#include "orderBook.hpp"
#include "order.hpp"

namespace TradingEngine::LimitOrderBook {
    PricePoint::PricePoint() {
        listStart = nullptr;
        listEnd = nullptr;
    }

    LimitOrderBook::LimitOrderBook(uint32_t symbolId, bool printLogs) {
        currOrderId = 0;
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

    uint64_t LimitOrderBook::addOrder(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (type == TradingEngine::Order::OrderType::LIMIT) {
            return processLimit(symbolId, userId, type, side, lifetime, price, quantity);
        } else {
            /* Only supports limit orders RN. */
            return INVALID_ORDER_ID;
        }
    }

    void LimitOrderBook::printOrder(TradingEngine::Order::Order& o) {
        constexpr std::string_view orderTypes[] = { "LIMIT", "MARKET" };
        constexpr std::string_view orderSides[] = { "BUY", "SELL" };
        constexpr std::string_view orderLifetimes[] = {
            "FOK",
            "IOC",
            "GFD",
            "GTD",
            "GTC"
        };

        std::cout << "  ORDER ID: " << o.id << '\n';
        std::cout << "  SIDE: " << orderSides[(uint8_t)o.side] << '\n';
        std::cout << "  PRICE: " << o.price << '\n';
        std::cout << "  QUANTITY: " << o.quantity << std::endl;
        std::cout << std::endl;
    }

    void LimitOrderBook::printList(std::shared_ptr<BookEntry> curr) {
        while (curr != nullptr) {
            TradingEngine::Order::Order o = *(curr->order);
            printOrder(o);
            curr = curr->next;
        }
    }

    std::string getTime() {
        using std::chrono::system_clock;
        auto currentTime = std::chrono::system_clock::now();
        char buffer[80];

        auto transformed = currentTime.time_since_epoch().count() / 1000000;

        auto millis = transformed % 1000;

        std::time_t tt;
        tt = system_clock::to_time_t ( currentTime );
        auto timeinfo = localtime (&tt);
        strftime (buffer,80,"%H:%M:%S",timeinfo);
        sprintf(buffer, "%s:%03d",buffer,(int)millis);

        return std::string(buffer);
    }

    void LimitOrderBook::executeTrade(const std::shared_ptr<TradingEngine::Order::Order>& order1, const std::shared_ptr<TradingEngine::Order::Order>& order2, uint64_t quantity, int64_t price) {
        uint64_t buyer = (order1->side == TradingEngine::Order::OrderSide::BUY) ? order1->id : order2->id;
        uint64_t seller = (order1->side == TradingEngine::Order::OrderSide::SELL) ? order1->id : order2->id;

        if (printLogs) {
            std::cout << getTime() << ": " << order1->symbolId << " TRADE " << quantity << " @ " << price << std::endl;
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

    void LimitOrderBook::insertOrder(std::shared_ptr<PricePoint> pricePoint, const std::shared_ptr<TradingEngine::Order::Order>& order) {
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

    uint64_t LimitOrderBook::processLimitBuy(const std::shared_ptr<TradingEngine::Order::Order>& order) {
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

                    return currOrderId;
                }
            }

            pricePoint->listStart = nullptr;
            updateMinAsk();
        }

        order->isActive = true;
        bidPrices.insert(order->price);
        insertOrder(pricePoints[(size_t)order->price], order);
        updateMaxBuy();

        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimitSell(const std::shared_ptr<TradingEngine::Order::Order>& order) {
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

                    return currOrderId;
                }
            }

            pricePoint->listStart = nullptr;
            updateMaxBuy();
        }

        order->isActive = true;
        askPrices.insert(order->price);
        insertOrder(pricePoints[(size_t)order->price], order);
        updateMinAsk();
        return currOrderId;
    }

    uint64_t LimitOrderBook::processLimit(uint32_t symbolId, uint64_t userId, TradingEngine::Order::OrderType type, TradingEngine::Order::OrderSide side,
        TradingEngine::Order::OrderLifetime lifetime, int64_t price, uint32_t quantity) {
        if (quantity <= 0) {
            throw std::runtime_error("ERROR: INVALID ORDER QUANTITY");
        }

        currOrderId++;
        std::shared_ptr<TradingEngine::Order::Order> order = std::make_shared<TradingEngine::Order::Order>(
            currOrderId,
            symbolId,
            userId,
            type,
            side,
            lifetime,
            price,
            quantity,
            false
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
        return order->id;
    }

    void LimitOrderBook::destroy() {
    }
}