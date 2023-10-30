#include "server.hpp"
using namespace TradingEngine;

Server::Server(std::string exchangeOption): exchange {*(TradingEngine::Util::ExchangeFactory::getExchange(exchangeOption))} {
    std::cout << "Created Exchange" << std::endl;
}

void Server::start() {
    std::cout << "Starting server..." << std::endl;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/login/<string>/<int>")
    ([this](std::string institution, int balance)
    {
        return "Added Trader " + institution + ": " + std::to_string(exchange.addTrader(institution, balance));
    });

    CROW_ROUTE(app, "/buy/<int>/<string>/<int>/<int>/")
    ([this](int traderId, std::string symbol, int price, int vol)
    {
        uint32_t id = exchange.getInstrumentId(symbol);
        auto type = TradingEngine::Order::OrderType::LIMIT;
        auto side = TradingEngine::Order::OrderSide::BUY;
        auto lifetime = TradingEngine::Order::OrderLifetime::FOK;
        exchange.sendOrder(id, traderId, type, side, lifetime, price, vol);
        return "Sent order: BUY " + symbol + " " + std::to_string(vol) + " @ " + std::to_string(price);
    });

    CROW_ROUTE(app, "/sell/<int>/<string>/<int>/<int>/")
    ([this](int traderId, std::string symbol, int price, int vol)
    {
        uint32_t id = exchange.getInstrumentId(symbol);
        auto type = TradingEngine::Order::OrderType::LIMIT;
        auto side = TradingEngine::Order::OrderSide::SELL;
        auto lifetime = TradingEngine::Order::OrderLifetime::FOK;
        exchange.sendOrder(id, traderId, type, side, lifetime, price, vol);
        return "Sent order: SELL " + symbol + " " + std::to_string(vol) + " @ " + std::to_string(price);
    });


    app.port(SERVER_PORT).run();
    app.wait_for_server_start();
}

void Server::stop() {
    exchange.destroy();
}