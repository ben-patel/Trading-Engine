#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include "../orders/order.hpp"
#include "../orders/orderBook.hpp"

/* Extremely limited unit tests, mostly been doing system tests */
class testBook : public ::testing::Test {
protected:
    TradingEngine::Order::OrderLifetime lifetimeGTC = TradingEngine::Order::OrderLifetime::GTC;
    TradingEngine::Order::OrderSide buy = TradingEngine::Order::OrderSide::BUY;
    TradingEngine::Order::OrderSide sell = TradingEngine::Order::OrderSide::SELL;
    TradingEngine::Order::OrderType limit = TradingEngine::Order::OrderType::LIMIT;
    TradingEngine::Order::OrderType market = TradingEngine::Order::OrderType::MARKET;
    uint32_t symbolId = 0;
    uint32_t userId = 1;
    uint64_t price = 10;
    uint64_t quantity = 100;
};

TEST_F(testBook, TestSuccessMatch) {
    ::testing::internal::CaptureStdout();

    // TradingEngine::LimitOrderBook::LimitOrderBook book {2, true};
    // book.processLimit(2, 0, 1, limit, buy, lifetimeGTC, 10, 100);
    // book.processLimit(2, 1, 2, limit, buy, lifetimeGTC, 10, 50);
    // book.processLimit(2, 2, 3, limit, sell, lifetimeGTC, 10, 10);

    // std::string output = ::testing::internal::GetCapturedStdout();
    //ASSERT_EQ(output, "ORDER ID 1 BUY 10 @ 10\nORDER ID 3 SELL 10 @ 10\n\n");
}

TEST_F(testBook, TestMutlipleOrder) {
    // TradingEngine::LimitOrderBook::LimitOrderBook book {2, true};

    // TradingEngine::Order::OrderType type = TradingEngine::Order::OrderType::LIMIT;
    // TradingEngine::Order::OrderSide side = TradingEngine::Order::OrderSide::BUY;
    // TradingEngine::Order::OrderLifetime lifetime = TradingEngine::Order::OrderLifetime::GFD;

    // ::testing::internal::CaptureStdout();
    // book.processLimit(2, 0, 1, type, side, lifetime, 10, 100);
    // book.processLimit(2, 1, 2, type, side, lifetime, 10, 50);
    // book.processLimit(2, 2, 3, TradingEngine::Order::OrderType::LIMIT,
    //     TradingEngine::Order::OrderSide::SELL,
    //     TradingEngine::Order::OrderLifetime::GFD, 10, 110);
    // book.processLimit(2, 3, 3, TradingEngine::Order::OrderType::LIMIT,
    //     TradingEngine::Order::OrderSide::SELL,
    //     TradingEngine::Order::OrderLifetime::GFD, 10, 100);
    // book.processLimit(2, 4, 3, TradingEngine::Order::OrderType::LIMIT,
    //     TradingEngine::Order::OrderSide::SELL,
    //     TradingEngine::Order::OrderLifetime::GFD, 10, 100);
    // book.processLimit(2, 5, 3, TradingEngine::Order::OrderType::LIMIT,
    //     TradingEngine::Order::OrderSide::BUY,
    //     TradingEngine::Order::OrderLifetime::GFD, 10, 100);

    // book.cancelOrder(3, 4);

    // book.processLimit(2, 6, 3, TradingEngine::Order::OrderType::LIMIT,
    //     TradingEngine::Order::OrderSide::BUY,
    //     TradingEngine::Order::OrderLifetime::GFD, 10, 100);

    std::string output = ::testing::internal::GetCapturedStdout();
    std::string expected = "ORDER ID 1 BUY 100 @ 10\nORDER ID 3 SELL 100 @ 10\n\nORDER ID 2 BUY 10 @ 10\nORDER ID 3 SELL 10 @ 10\n"
                    "\nORDER ID 2 BUY 40 @ 10\nORDER ID 4 SELL 40 @ 10\n\nORDER ID 6 BUY 60 @ 10\nORDER ID 4 SELL 60 @ 10\n"
                    "\nORDER ID 6 BUY 40 @ 10\nORDER ID 5 SELL 40 @ 10\n\n";

    ASSERT_EQ(output, expected);
}

TEST_F(testBook, TestSingleOrder) {
    TradingEngine::LimitOrderBook::LimitOrderBook book {2, true};
    //ASSERT_NE(book.addOrder(symbolId, 1, userId, limit, buy, lifetimeGTC, price, quantity), TradingEngine::LimitOrderBook::INVALID_ORDER_ID);
}

TEST_F(testBook, TestInvalidType) {
    TradingEngine::LimitOrderBook::LimitOrderBook book {2, true};
    //ASSERT_EQ(book.addOrder(symbolId, 1, userId, market, buy, lifetimeGTC, price, quantity), TradingEngine::LimitOrderBook::INVALID_ORDER_ID);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
